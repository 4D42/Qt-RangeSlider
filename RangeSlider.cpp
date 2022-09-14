#include "rangeslider.h"
#include <QDebug>
#include <QFrame>

namespace
{

const int scHandleSideLength = 8;
const int scHandleSideMarging = -8;
const int scHandlBorderRadius = 4;
const int scHandlBorderWidth = 1;

const QColor scHandlBorderColor = QColor(0x40, 0x3F, 0x3F);
const QColor scHandleColor = QColor(0x78, 0x78, 0x78);
const QColor scHandleColorHovering = QColor(0x17, 0x9A, 0xE0);

const int scSliderBarHeight = 4;
const int scGrooveBorderWidth = 1;
const int scLeftRightMargin = 5;
const int grooveBorderRadius = 0;

const QColor scGrouveBorderColor = QColor(0x40, 0x3F, 0x3F);
const QColor scGrooveColor = QColor(0x40, 0x3F, 0x3F);

}

RangeSlider::RangeSlider(QWidget* aParent)
    : RangeSlider(Qt::Horizontal, DoubleHandles, aParent)
{
}

RangeSlider::RangeSlider(Qt::Orientation ori, Options t, QWidget* aParent)
    : QFrame(aParent),
      orientation(ori),
      type(t)
{
    mMinimum = 0;
    mMaximum = 100;
    mLowerValue = 0;
    mUpperValue = 100;
    mInterval = (mMaximum - mMinimum);
    mFirstHandlePressed = false;
    mSecondHandlePressed = false;
    mBackgroudColorEnabled  = QColor(0x33, 0x75, 0xA3);
    mBackgroudColorDisabled = Qt::darkGray;
    mBackgroudColor = mBackgroudColorEnabled;
    m1HandleColor = scHandleColor;
    m2HandleColor = scHandleColor;

    mHandleWidth = scHandleSideLength+scHandlBorderWidth;
    mHandleHeight = (scSliderBarHeight-scHandlBorderWidth)-2*scHandleSideMarging;

    /* widget option */
    setMouseTracking(true);

}

void RangeSlider::paintEvent(QPaintEvent* aEvent)
{

    Q_UNUSED(aEvent);
    QPainter painter(this);

    // Background Groove
    if(orientation == Qt::Horizontal)
        mRectGroove = QRectF(scLeftRightMargin, (height() - scSliderBarHeight) / 2, width() - scLeftRightMargin * 2 , scSliderBarHeight+scGrooveBorderWidth);
    else
        mRectGroove = QRectF((width() - scSliderBarHeight) / 2, scLeftRightMargin, scSliderBarHeight+scGrooveBorderWidth, height() - scLeftRightMargin*2);

    // Draw the groove
    QPen pen;
    pen.setColor(scGrouveBorderColor);
    pen.setWidth(scGrooveBorderWidth);
    painter.setPen(pen);
    QBrush backgroundBrush(scGrooveColor);
    painter.setBrush(backgroundBrush);
    painter.drawRoundedRect(mRectGroove, grooveBorderRadius, grooveBorderRadius);


    //Draw left handle
    pen.setColor(scHandlBorderColor);
    pen.setWidth(scHandlBorderWidth);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing);
    QBrush handle1Brush(m1HandleColor);
    painter.setBrush(handle1Brush);
    mLeftHandleRect = firstHandleRect();
    if(type.testFlag(LeftHandle))
        painter.drawRoundedRect(mLeftHandleRect, scHandlBorderRadius, scHandlBorderRadius);

    //Draw Right handle
    QBrush handle2Brush(m2HandleColor);
    painter.setBrush(handle2Brush);
    mRightHandleRect = secondHandleRect();
    if(type.testFlag(RightHandle))
        painter.drawRoundedRect(mRightHandleRect, scHandlBorderRadius, scHandlBorderRadius);

    //Draw Groove Betwen Handles
    painter.setRenderHint(QPainter::Antialiasing, false);

    mSelectedRect = mRectGroove;
    double left;
    double right;

    if(orientation == Qt::Horizontal) {
        left = ((type.testFlag(LeftHandle) ? mLeftHandleRect.right() : mRectGroove.left()));
        right = ((type.testFlag(RightHandle) ? mRightHandleRect.left()-scGrooveBorderWidth : mRectGroove.right()-scGrooveBorderWidth));
    } else {
        left = ((type.testFlag(LeftHandle) ? mLeftHandleRect.bottom() : mRectGroove.top()));
        right = ((type.testFlag(RightHandle) ? mRightHandleRect.top()-scGrooveBorderWidth : mRectGroove.bottom()-scGrooveBorderWidth));
    }

    if((right-left) > 0) {
        if(orientation == Qt::Horizontal) {
            mSelectedRect.setLeft(left);
            mSelectedRect.setRight(right);
        } else {
            mSelectedRect.setBottom(left);
            mSelectedRect.setTop(right);
        }

        QBrush selectedBrush(mBackgroudColor);
        painter.setBrush(selectedBrush);
        painter.drawRect(mSelectedRect);
    }
}

QRectF RangeSlider::firstHandleRect() const
{
    float percentage = (mLowerValue - mMinimum) * 1.0 / mInterval;
    double aValue = (percentage * validLength());

    QRectF rec;
    if(orientation == Qt::Horizontal){
        rec = QRectF(mRectGroove.left()+mHandleWidth+aValue-mHandleWidth, mRectGroove.top()+mRectGroove.height()/2.0 - mHandleHeight/2.0, mHandleWidth, mHandleHeight);
        rec.moveCenter(QPointF(mRectGroove.left()+aValue,mRectGroove.center().y()));
    }else
        rec = QRectF((width()-((scSliderBarHeight-2*scHandlBorderWidth)-2*scHandleSideMarging)) / 2, aValue, ((scSliderBarHeight-2*scHandlBorderWidth)-2*scHandleSideMarging), scHandleSideLength-2*scHandlBorderWidth);

    return rec;
}

QRectF RangeSlider::secondHandleRect() const
{
    float percentage = (mUpperValue - mMinimum) * 1.0 / mInterval;
    double aValue = (percentage * validLength());

    QRectF rec;
    if(orientation == Qt::Horizontal) {
        rec = QRectF(mRectGroove.left()+mHandleWidth+aValue, mRectGroove.top()+mRectGroove.height()/2.0 - mHandleHeight/2.0, mHandleWidth, mHandleHeight);
        rec.moveCenter(QPointF(mRectGroove.left()+aValue,mRectGroove.center().y()));
    } else
        rec = QRectF((width()-((scSliderBarHeight-2*scHandlBorderWidth)-2*scHandleSideMarging)) / 2, aValue, ((scSliderBarHeight-2*scHandlBorderWidth)-2*scHandleSideMarging), scHandleSideLength-2*scHandlBorderWidth);

    return rec;
}

/******************************************************* EVENTS FUNCTIONS ********************************************************************************/

void RangeSlider::mousePressEvent(QMouseEvent* aEvent)
{
    if(aEvent->buttons() & Qt::LeftButton)
    {
        int posValue, firstHandleRectPosValue, secondHandleRectPosValue;
        posValue = (orientation == Qt::Horizontal) ? aEvent->pos().x() : aEvent->pos().y();
        firstHandleRectPosValue = (orientation == Qt::Horizontal) ? mLeftHandleRect.center().x() : mLeftHandleRect.center().y();
        secondHandleRectPosValue = (orientation == Qt::Horizontal) ?  mRightHandleRect.center().x() : mRightHandleRect.center().y();

        mSecondHandlePressed = mRightHandleRect.contains(aEvent->pos());
        mFirstHandlePressed = !mSecondHandlePressed && mLeftHandleRect.contains(aEvent->pos());

        /* Check in case the two handles are overlapping at the maximum position so handles don't get stuck*/
        if(mSecondHandlePressed && mUpperValue == mMaximum) {
            QRectF intersection = mRightHandleRect.intersected(mLeftHandleRect);
            double intersectionPersentage =  (intersection.width()*intersection.height()) / (mRightHandleRect.width()*mRightHandleRect.height());
            if (intersectionPersentage > 0.8) {
                mFirstHandlePressed = true;
                mSecondHandlePressed = false;
            }
        }

        if(mFirstHandlePressed)
        {
            mDelta = posValue - firstHandleRectPosValue;
        }
        else if(mSecondHandlePressed)
        {
            mDelta = posValue - secondHandleRectPosValue;
        }
    }

}

void RangeSlider::mouseMoveEvent(QMouseEvent* aEvent)
{
    if(mLeftHandleRect.contains(aEvent->pos()) && m1HandleColor != scHandleColorHovering ){
        m1HandleColor = scHandleColorHovering;
        update();
    } else if(!mLeftHandleRect.contains(aEvent->pos()) && !mFirstHandlePressed && m1HandleColor != scHandleColor){
        m1HandleColor = scHandleColor;
        update();
    }else if(mRightHandleRect.contains(aEvent->pos()) && m2HandleColor != scHandleColorHovering){
        m2HandleColor = scHandleColorHovering;
        update();
    } else if(!mRightHandleRect.contains(aEvent->pos()) && !mSecondHandlePressed && m2HandleColor != scHandleColor){
        m2HandleColor = scHandleColor;
        update();
    }

    if(aEvent->buttons() & Qt::LeftButton)
    {
        double posValue, startGrooveCoord;
        posValue = (orientation == Qt::Horizontal) ? aEvent->pos().x() : aEvent->pos().y();
        startGrooveCoord = (orientation == Qt::Horizontal) ? mRectGroove.left() : mRectGroove.bottom();

        double newPosition = (posValue - mDelta);

        if(mFirstHandlePressed && type.testFlag(LeftHandle)) {
            double persentage = (newPosition - startGrooveCoord)/(1.0 * validLength()); //persentage position in groove
            onSetLowerValue(round(persentage*mInterval));
        }
        else if(mSecondHandlePressed && type.testFlag(RightHandle)) {
            double persentage = (newPosition - startGrooveCoord)/(1.0 * validLength()); //persentage position in groove
            onSetUpperValue(round(persentage*mInterval));
        }

    }
}

void RangeSlider::mouseReleaseEvent(QMouseEvent* aEvent)
{
    Q_UNUSED(aEvent);

    mFirstHandlePressed = false;
    mSecondHandlePressed = false;
}

void RangeSlider::wheelEvent(QWheelEvent* aEvent)
{
   int position, firstHandleRectPosValue, secondHandleRectPosValue;
   position = (orientation == Qt::Horizontal) ? aEvent->position().x() : aEvent->position().y();
   firstHandleRectPosValue = (orientation == Qt::Horizontal) ? mLeftHandleRect.center().x() : mLeftHandleRect.center().y();
   secondHandleRectPosValue = (orientation == Qt::Horizontal) ?  mRightHandleRect.center().x() : mRightHandleRect.center().y();

   if (abs(firstHandleRectPosValue - position) < abs(secondHandleRectPosValue - position)) {
       if(aEvent->angleDelta().y() < 0)
           setLowerValue(mLowerValue-1);
       else
          setLowerValue(mLowerValue+1);
   }else {
       if(aEvent->angleDelta().y() < 0)
           setUpperValue(mUpperValue-1);
       else
           setUpperValue(mUpperValue+1);
    }

}

void RangeSlider::changeEvent(QEvent* aEvent)
{
    if(aEvent->type() == QEvent::EnabledChange)
    {
        if(isEnabled())
        {
            mBackgroudColor = mBackgroudColorEnabled;
        }
        else
        {
            mBackgroudColor = mBackgroudColorDisabled;
        }
        update();
    }
}


void RangeSlider::leaveEvent(QEvent* aEvent)
{
    Q_UNUSED(aEvent);

    if(!mFirstHandlePressed)
    {
         m1HandleColor = scHandleColor;
         update();
    }

    if(!mSecondHandlePressed)
    {
         m2HandleColor = scHandleColor;
         update();
    }
}

/**************************************** GET FUNCTIONS ********************************************/

int RangeSlider::getMinimun() const
{
    return mMinimum;
}

int RangeSlider::getMaximun() const
{
    return mMaximum;
}

int RangeSlider::getLowerValue() const
{
    return mLowerValue;
}

int RangeSlider::getUpperValue() const
{
    return mUpperValue;
}


/********************************************** SET FUNCTIONS *******************************************************/


void RangeSlider::setLowerUpperValue(int aLowerValue, int aUpperValue)
{
    setLowerValue(aLowerValue);
    setUpperValue(aUpperValue);
}

void RangeSlider::setLowerValue(int aLowerValue)
{
    onSetLowerValue(aLowerValue);
}

void RangeSlider::setUpperValue(int aUpperValue)
{
    onSetUpperValue(aUpperValue);
}

void RangeSlider::setMaximum(int aMaximum)
{
    onSetMaximum(aMaximum);
}

void RangeSlider::setMinimum(int aMinimum)
{
    onSetMinimum(aMinimum);
}

void RangeSlider::setRange(int aMinimum, int mMaximum)
{
    onSetMinimum(aMinimum);
    onSetMaximum(mMaximum);
}

void RangeSlider::setOrientation(Qt::Orientation orientation)
{
    this->orientation = orientation;
}

void RangeSlider::setOptions(Options type)
{
    this->type = type;
}

/***************************************************************** SLOTS ******************************************************************************/

void RangeSlider::onSetLowerValue(int aLowerValue)
{
    if(aLowerValue > mMaximum)
    {
        aLowerValue = mMaximum;
    }

    if(aLowerValue < mMinimum)
    {
        aLowerValue = mMinimum;
    }

    if(type.testFlag(RightHandle) && aLowerValue > mUpperValue)
        aLowerValue = mUpperValue;

    if(mLowerValue != aLowerValue) {
        mLowerValue = aLowerValue;
        emit lowerValueChanged(mLowerValue);

        update();
    }
}

void RangeSlider::onSetUpperValue(int aUpperValue)
{
    if(aUpperValue > mMaximum)
    {
        aUpperValue = mMaximum;
    }

    if(aUpperValue < mMinimum)
    {
        aUpperValue = mMinimum;
    }

    if(type.testFlag(LeftHandle) && aUpperValue< mLowerValue)
        aUpperValue = mLowerValue;

    if(mUpperValue != aUpperValue) {
        mUpperValue = aUpperValue;
        emit upperValueChanged(mUpperValue);

        update();
    }
}

void RangeSlider::onSetMinimum(int aMinimum)
{
    if(aMinimum <= mMaximum)
    {
        mMinimum = aMinimum;
    }
    else
    {
        int oldMax = mMaximum;
        mMinimum = oldMax;
        mMaximum = aMinimum;
    }
    mInterval = mMaximum - mMinimum;
    update();

    if(mLowerValue<mMinimum)
        onSetLowerValue(mMinimum);

    if(mUpperValue>mMaximum)
        onSetUpperValue(mMaximum);

    emit rangeChanged(mMinimum, mMaximum);
}

void RangeSlider::onSetMaximum(int aMaximum)
{
    if(aMaximum >= mMinimum)
    {
        mMaximum = aMaximum;
    }
    else
    {
        int oldMin = mMinimum;
        mMaximum = oldMin;
        mMinimum = aMaximum;
    }
    mInterval = mMaximum - mMinimum;
    update();

    if(mLowerValue<mMinimum)
        onSetLowerValue(mMinimum);

    if(mUpperValue>mMaximum)
        onSetUpperValue(mMaximum);

    emit rangeChanged(mMinimum, mMaximum);
}

/***************************************************** OTHER FUNCTIONS ********************************************************************/
int RangeSlider::validLength() const
{
    int len = (orientation == Qt::Horizontal) ? mRectGroove.width() : mRectGroove.height();
    return len;
}

QSize RangeSlider::minimumSizeHint() const
{
    return QSize(scHandleSideLength * 2 + scLeftRightMargin * 2, scHandleSideLength);
}
