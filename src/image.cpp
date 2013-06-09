#include <image.h>
#include <utils.h>

namespace avlib
{

const char * ImageTypeString[] = {
/*[IMAGE_TYPE_UNKNOWN] 	=*/ "unknown",
/*[IMAGE_TYPE_YUV420]	=*/ "YUV420",
/*[IMAGE_TYPE_RGB]	=*/ "RGB",
/*[IMAGE_TYPE_ARGB]	=*/ "ARGB",
};

template <class T>
CImage<T>::CImage() :
	m_comp_num(0),
	m_comp(NULL),
	m_format(IMAGE_TYPE_UNKNOWN, 0, 0)
{
}

template <class T>
CImage<T>::CImage(CImageFormat format) :
	m_comp_num(0),
	m_comp(NULL),
	m_format(IMAGE_TYPE_UNKNOWN, 0, 0)
{
	setFormat(format);
}

template <class T>
CImage<T>::CImage(CSize size) :
	m_comp_num(0),
	m_comp(NULL),
	m_format(IMAGE_TYPE_UNKNOWN, 0, 0)
{
	setFormat(IMAGE_TYPE_RGB, size);
}

template <class T>
CImage<T>::CImage(enum ImageType type, CSize size) :
	m_comp_num(0),
	m_comp(NULL),
	m_format(IMAGE_TYPE_UNKNOWN, 0, 0)
{
	setFormat(type, size);
}

template <class T>
CImage<T>::CImage(enum ImageType type, int height, int width) :
	m_comp_num(0),
	m_comp(NULL),
	m_format(IMAGE_TYPE_UNKNOWN, 0, 0)
{
	setFormat(type, height, width);
}

template <class T>
CImage<T>::CImage(const CImage<T> & src)
{
	operator=(src);
}

template <class T>
CImage<T> & CImage<T>::operator=(const CImage<T> & src)
{
	if(m_format != src.m_format)
	{
		setFormat(src.m_format);
	}
	for(int i=0;i<m_comp_num;i++)
	{
		*m_comp[i] = *src.m_comp[i];
	}
	return *this;
}

template <class T>
template <class U> CImage<T>::CImage(const CImage<U> & src) :
	m_comp_num(0),
	m_comp(NULL),
	m_format(IMAGE_TYPE_UNKNOWN, 0, 0)
{
	operator=(src);
}

template <class T>
template <class U> CImage<T> & CImage<T>::operator=(const CImage<U> & src)
{
	if(m_format != src.m_format)
	{
		setFormat(src.m_format);
	}
	for(int i=0;i<m_comp_num;i++)
	{
		*m_comp[i] = *src.m_comp[i];
	}
	return *this;
}

template <class T>
CImage<T>::~CImage()
{
	if(NULL != m_comp)
	{
		release();
	}
}

template <class T>
bool CImage<T>::setFormat(enum ImageType type, CSize size)
{
	return setFormat(CImageFormat(type, size));
}

template <class T>
bool CImage<T>::setFormat(enum ImageType type, int height, int width)
{
	return setFormat(CImageFormat(type, height, width));
}

template <class T>
void CImage<T>::release()
{
	for(int i=0;i<m_comp_num;i++)
	{
		delete m_comp[i];
	}
	delete [] m_comp;
	m_comp_num = 0;
	m_comp = NULL;
}

template <class T>
void CImage<T>::alloc(int num)
{
	m_comp_num = num;
	m_comp = new CComponent<T>*[m_comp_num];
	for(int i=0;i<m_comp_num;i++)
	{
		m_comp[i] = new CComponent<T>();
	}
}

template <class T>
bool CImage<T>::setFormat(CImageFormat format)
{
	if(NULL != m_comp && m_format == format)
	{
		return true;
	} 
	else if(NULL != m_comp && m_format != format)
	{
		release();
	}
	if(NULL == m_comp)
	{
		switch(format.Type)
		{
		case IMAGE_TYPE_RGB:
		case IMAGE_TYPE_YUV420:
			alloc(3);
		break;
		default:
			throw utils::StringFormatException("unknown image type: %d\n", format.Type);
		}
	}
	switch(format.Type)
	{
	case IMAGE_TYPE_RGB:
		for(int i=0;i<m_comp_num;i++)
		{
			m_comp[i]->setSize(format.Size.Height, format.Size.Width);
		}
	break;
	case IMAGE_TYPE_YUV420:
		if(m_comp_num != 3)
		{
			throw utils::StringFormatException("IMAGE_TYPE_YUV420 and m_comp_num == %d\n", m_comp_num);
		}
		m_comp[0]->setSize(format.Size.Height, format.Size.Width);
		m_comp[1]->setSize(format.Size.Height/2, format.Size.Width/2);
		m_comp[2]->setSize(format.Size.Height/2, format.Size.Width/2);
	break;
	default:
		return false;
	}
	m_format = format;
	return true;
}

template <class T>
int CImage<T>::getComponents(void)
{
	return m_comp_num;
}

template <class T>
CComponent<T> & CImage<T>::operator[](int index)
{
	if(index >= 0 && index < m_comp_num)
	{
		return *m_comp[index];
	}
	else
	{
		throw std::exception();
	}
}

template <class T>
CImageFormat CImage<T>::getFormat(void)
{
	return m_format;
}

INSTANTIATE(CImage, uint8_t);
INSTANTIATE(CImage, uint16_t);
INSTANTIATE(CImage, int16_t);
INSTANTIATE(CImage, int32_t);
INSTANTIATE(CImage, float);
CONVERSION(CImage, int16_t, float);
CONVERSION(CImage, float, int16_t);
CONVERSION(CImage, int32_t, float);
CONVERSION(CImage, float, int32_t);
CONVERSION(CImage, uint8_t, float);
CONVERSION(CImage, float, uint8_t);
CONVERSION(CImage, uint16_t, uint8_t);
CONVERSION(CImage, uint8_t, uint16_t);
}
