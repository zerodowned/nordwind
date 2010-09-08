#include "OSIHandler.hpp"

#include <QtGui/qimage.h>
#include <QtCore/qvariant.h>
#include <QtGui/qcolor.h>

QT_BEGIN_NAMESPACE

class OSIHandlerPrivate
{
    Q_DECLARE_PUBLIC(OSIHandler)
    public:
    bool haveReadNone;
    bool haveReadAll;
    mng_handle hMNG;
    QImage image;
    int elapsed;
    int nextDelay;
    int iterCount;
    int frameIndex;
    int nextIndex;
    int frameCount;
    mng_uint32 iStyle;
    mng_bool readData(mng_ptr pBuf, mng_uint32 iSize, mng_uint32p pRead);
    mng_bool writeData(mng_ptr pBuf, mng_uint32 iSize, mng_uint32p pWritten);
    mng_bool processHeader(mng_uint32 iWidth, mng_uint32 iHeight);
    OSIHandlerPrivate(OSIHandler *q_ptr);
    ~OSIHandlerPrivate();
    bool getNextImage(QImage *result);
    bool writeImage(const QImage &image);
    int currentImageNumber() const;
    int imageCount() const;
    bool jumpToImage(int imageNumber);
    bool jumpToNextImage();
    int nextImageDelay() const;
    bool setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;
    OSIHandler *q_ptr;
};

OSIHandlerPrivate::OSIHandlerPrivate(OSIHandler *q_ptr)
    : haveReadNone(true), haveReadAll(false), elapsed(0), nextDelay(0), iterCount(1),
      frameIndex(-1), nextIndex(0), frameCount(0), q_ptr(q_ptr)
{
    iStyle = (QSysInfo::ByteOrder == QSysInfo::LittleEndian) ? MNG_CANVAS_BGRA8 : MNG_CANVAS_ARGB8;
    // Initialize libmng
    hMNG = mng_initialize((mng_ptr)this, myalloc, myfree, mytrace);
    if (hMNG) {
        // Set callback functions
        mng_setcb_errorproc(hMNG, myerror);
        mng_setcb_openstream(hMNG, myopenstream);
        mng_setcb_closestream(hMNG, myclosestream);
        mng_setcb_readdata(hMNG, myreaddata);
        mng_setcb_writedata(hMNG, mywritedata);
        mng_setcb_processheader(hMNG, myprocessheader);
        mng_setcb_getcanvasline(hMNG, mygetcanvasline);
        mng_setcb_refresh(hMNG, myrefresh);
        mng_setcb_gettickcount(hMNG, mygettickcount);
        mng_setcb_settimer(hMNG, mysettimer);
        mng_setcb_processterm(hMNG, myprocessterm);
        mng_set_doprogressive(hMNG, MNG_FALSE);
        mng_set_suspensionmode(hMNG, MNG_TRUE);
    }
}

OSIHandlerPrivate::~OSIHandlerPrivate()
{
    mng_cleanup(&hMNG);
}

mng_bool OSIHandlerPrivate::readData(mng_ptr pBuf, mng_uint32 iSize, mng_uint32p pRead)
{
    Q_Q(OSIHandler);
    *pRead = q->device()->read((char *)pBuf, iSize);
    return (*pRead > 0) ? MNG_TRUE : MNG_FALSE;
}

mng_bool OSIHandlerPrivate::writeData(mng_ptr pBuf, mng_uint32 iSize, mng_uint32p pWritten)
{
    Q_Q(OSIHandler);
    *pWritten = q->device()->write((char *)pBuf, iSize);
    return MNG_TRUE;
}

mng_bool OSIHandlerPrivate::processHeader(mng_uint32 iWidth, mng_uint32 iHeight)
{
    if (mng_set_canvasstyle(hMNG, iStyle) != MNG_NOERROR)
        return MNG_FALSE;
    image = QImage(iWidth, iHeight, QImage::Format_ARGB32);
    image.fill(0);
    return MNG_TRUE;
}

bool OSIHandlerPrivate::getNextImage(QImage *result)
{
    mng_retcode ret;
    if (haveReadNone) {
        haveReadNone = false;
        ret = mng_readdisplay(hMNG);
    } else {
        ret = mng_display_resume(hMNG);
    }
    if ((MNG_NOERROR == ret) || (MNG_NEEDTIMERWAIT == ret)) {
        *result = image;
        image.fill(0);
        frameIndex = nextIndex++;
        if (haveReadAll && (frameCount == 0))
            frameCount = nextIndex;
        return true;
    }
    return false;
}

bool OSIHandlerPrivate::writeImage(const QImage &image)
{
    mng_reset(hMNG);
    if (mng_create(hMNG) != MNG_NOERROR)
        return false;

    this->image = image.convertToFormat(QImage::Format_ARGB32);
    int w = image.width();
    int h = image.height();

    if (
    // width, height, ticks, layercount, framecount, playtime, simplicity
         (mng_putchunk_mhdr(hMNG, w, h, 1000, 0, 0, 0, 7) == MNG_NOERROR) &&
    // termination_action, action_after_iterations, delay, iteration_max
         (mng_putchunk_term(hMNG, 3, 0, 1, 0x7FFFFFFF) == MNG_NOERROR) &&
    // width, height, bitdepth, colortype, compression, filter, interlace
         (mng_putchunk_ihdr(hMNG, w, h, 8, 6, 0, 0, 0) == MNG_NOERROR) &&
    // width, height, colortype, bitdepth, compression, filter, interlace, canvasstyle, getcanvasline
         (mng_putimgdata_ihdr(hMNG, w, h, 6, 8, 0, 0, 0, iStyle, mygetcanvasline) == MNG_NOERROR) &&
         (mng_putchunk_iend(hMNG) == MNG_NOERROR) &&
         (mng_putchunk_mend(hMNG) == MNG_NOERROR) &&
         (mng_write(hMNG) == MNG_NOERROR)
        )
        return true;
    return false;
}

int OSIHandlerPrivate::currentImageNumber() const
{
//    return mng_get_currentframe(hMNG) % imageCount(); not implemented, apparently
    return frameIndex;
}

int OSIHandlerPrivate::imageCount() const
{
//    return mng_get_totalframes(hMNG); not implemented, apparently
    if (haveReadAll)
        return frameCount;
    return 0; // Don't know
}

bool OSIHandlerPrivate::jumpToImage(int imageNumber)
{
    if (imageNumber == nextIndex)
        return true;

    if ((imageNumber == 0) && haveReadAll && (nextIndex == frameCount)) {
        // Loop!
        nextIndex = 0;
        return true;
    }
    if (mng_display_freeze(hMNG) == MNG_NOERROR) {
        if (mng_display_goframe(hMNG, imageNumber) == MNG_NOERROR) {
            nextIndex = imageNumber;
            return true;
        }
    }
    return false;
}

bool OSIHandlerPrivate::jumpToNextImage()
{
    return jumpToImage((currentImageNumber()+1) % imageCount());
}

int OSIHandlerPrivate::nextImageDelay() const
{
    return nextDelay;
}

bool OSIHandlerPrivate::setBackgroundColor(const QColor &color)
{
    mng_uint16 iRed = (mng_uint16)(color.red() << 8);
    mng_uint16 iBlue = (mng_uint16)(color.blue() << 8);
    mng_uint16 iGreen = (mng_uint16)(color.green() << 8);
    return (mng_set_bgcolor(hMNG, iRed, iBlue, iGreen) == MNG_NOERROR);
}

QColor OSIHandlerPrivate::backgroundColor() const
{
    mng_uint16 iRed;
    mng_uint16 iBlue;
    mng_uint16 iGreen;
    if (mng_get_bgcolor(hMNG, &iRed, &iBlue, &iGreen) == MNG_NOERROR)
        return QColor((iRed >> 8) & 0xFF, (iGreen >> 8) & 0xFF, (iBlue >> 8) & 0xFF);
    return QColor();
}

OSIHandler::OSIHandler()
    : d_ptr(new OSIHandlerPrivate(this))
{
}

OSIHandler::~OSIHandler()
{
}

/*! \reimp */
bool OSIHandler::canRead() const
{
    Q_D(const OSIHandler);
    if (!d->haveReadNone)
        return (!d->haveReadAll || (d->haveReadAll && (d->nextIndex < d->frameCount)));

    if (canRead(device())) {
        setFormat("mng");
        return true;
    }
    return false;
}

/*! \internal */
bool OSIHandler::canRead(QIODevice *device)
{
    if (!device) {
        qWarning("QMngHandler::canRead() called with no device");
        return false;
    }

    return device->peek(8) == "\x8A\x4D\x4E\x47\x0D\x0A\x1A\x0A";
}

/*! \reimp */
QByteArray OSIHandler::name() const
{
    return "mng";
}

/*! \reimp */
bool OSIHandler::read(QImage *image)
{
    Q_D(OSIHandler);
    return canRead() ? d->getNextImage(image) : false;
}

/*! \reimp */
bool OSIHandler::write(const QImage &image)
{
    Q_D(OSIHandler);
    return d->writeImage(image);
}

/*! \reimp */
int OSIHandler::currentImageNumber() const
{
    Q_D(const OSIHandler);
    return d->currentImageNumber();
}

/*! \reimp */
int OSIHandler::imageCount() const
{
    Q_D(const OSIHandler);
    return d->imageCount();
}

/*! \reimp */
bool OSIHandler::jumpToImage(int imageNumber)
{
    Q_D(OSIHandler);
    return d->jumpToImage(imageNumber);
}

/*! \reimp */
bool OSIHandler::jumpToNextImage()
{
    Q_D(OSIHandler);
    return d->jumpToNextImage();
}

/*! \reimp */
int OSIHandler::loopCount() const
{
    Q_D(const OSIHandler);
    if (d->iterCount == 0x7FFFFFFF)
        return -1; // infinite loop
    return d->iterCount-1;
}

/*! \reimp */
int OSIHandler::nextImageDelay() const
{
    Q_D(const OSIHandler);
    return d->nextImageDelay();
}

/*! \reimp */
QVariant OSIHandler::option(ImageOption option) const
{
    Q_D(const OSIHandler);
    if (option == QImageIOHandler::Animation)
        return true;
    else if (option == QImageIOHandler::BackgroundColor)
        return d->backgroundColor();
    return QVariant();
}

/*! \reimp */
void OSIHandler::setOption(ImageOption option, const QVariant & value)
{
    Q_D(OSIHandler);
    if (option == QImageIOHandler::BackgroundColor)
        d->setBackgroundColor(qVariantValue<QColor>(value));
}

/*! \reimp */
bool OSIHandler::supportsOption(ImageOption option) const
{
    if (option == QImageIOHandler::Animation)
        return true;
    else if (option == QImageIOHandler::BackgroundColor)
        return true;
    return false;
}

QT_END_NAMESPACE
