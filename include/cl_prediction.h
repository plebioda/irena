#ifndef _CL_PREDICTION_H
#define _CL_PREDICTION_H

#include <utils.h>
#include <avlib.h>
#include <prediction.h>
#include <cl_device.h>
#include <cl_image.h>

namespace avlib
{

class CCLPrediction : public CPrediction
{
public:
	CCLPrediction(CCLDevice * dev);
	CCLPrediction(CCLDevice * dev, CImageFormat format);
protected:
	CCLDevice * m_dev;
};

}

#endif //_CL_PREDICTION_H