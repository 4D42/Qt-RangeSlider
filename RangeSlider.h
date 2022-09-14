#pragma once

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QFrame>

class RangeSlider : public QFrame
{
    Q_OBJECT
    Q_ENUMS(RangeSliderTypes)

public:
    enum Option {
        NoHandle = 0x0,
        LeftHandle = 0x1,
        RightHandle = 0x2,
        DoubleHandles = LeftHandle | RightHandle
    };
    Q_DECLARE_FLAGS(Options, Option)

    RangeSlider( QWidget* aParent = Q_NULLPTR);
    RangeSlider( Qt::Orientation ori, Options t = DoubleHandles, QWidget* aParent = Q_NULLPTR);

    QSize minimumSizeHint() const override;

    int getMinimun() const;
    void setMinimum(int aMinimum);

    int getMaximun() const;
    void setMaximum(int aMaximum);

    int getLowerValue() const;
    void setLowerValue(int aLowerValue);

    int getUpperValue() const;
    void setUpperValue(int aUpperValue);

    void setLowerUpperValue(int aLowerValue, int aUpperValue);
    void setRange(int aMinimum, int aMaximum);

    void setOrientation(Qt::Orientation orientation);
    void setOptions(Options type);



protected:
    void paintEvent(QPaintEvent* aEvent) override;
    void mousePressEvent(QMouseEvent* aEvent) override;
    void mouseMoveEvent(QMouseEvent* aEvent) override;
    void mouseReleaseEvent(QMouseEvent* aEvent) override;
    void wheelEvent(QWheelEvent *event) override;
    void changeEvent(QEvent* aEvent) override;
    void leaveEvent(QEvent *aEvent) override;

    QRectF firstHandleRect() const;
    QRectF secondHandleRect() const;
    QRectF handleRect(int aValue) const;

signals:
    void lowerValueChanged(int aLowerValue);
    void upperValueChanged(int aUpperValue);
    void rangeChanged(int aMin, int aMax);

public slots:
    void onSetLowerValue(int aLowerValue);
    void onSetUpperValue(int aUpperValue);
    void onSetMinimum(int aMinimum);
    void onSetMaximum(int aMaximum);

private:
    Q_DISABLE_COPY(RangeSlider)
    float currentPercentage();
    int validLength() const;

    int mMinimum;
    int mMaximum;
    int mLowerValue;
    int mUpperValue;
    bool mFirstHandlePressed;
    bool mSecondHandlePressed;
    int mInterval;
    int mDelta;

    int mHandleWidth;
    int mHandleHeight;

    QColor mBackgroudColorEnabled;
    QColor mBackgroudColorDisabled;
    QColor mBackgroudColor;
    QColor m1HandleColor;
    QColor m2HandleColor;
    Qt::Orientation orientation;
    Options type;

    QRectF mRectGroove;
    QRectF mSelectedRect;
    QRectF mLeftHandleRect;
    QRectF mRightHandleRect;

    int test;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(RangeSlider::Options)
