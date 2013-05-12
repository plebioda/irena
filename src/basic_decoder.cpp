#include <basic_decoder.h>

#include <utils.h>
#include <log.h>

namespace avlib
{

CBasicDecoder::CBasicDecoder()
{
}

CBasicDecoder::~CBasicDecoder()
{
}

bool CBasicDecoder::Decode(CBitstream * pBstr, CSequence * pSeq)
{	
	struct sos_marker sos;
	pBstr->read_block(&sos, sizeof(sos));
	if(MARKER_TYPE_SOS != sos.type)
	{
		throw utils::StringFormatException("wrong stream format [can not find start of stream marker]\n");
	}
	dbg("Start of Sequence marker:\n");
	dbg("Frames: %d\n", sos.frames_number);
	dbg("Width : %d\n", sos.width);
	dbg("Height: %d\n", sos.height);
	pSeq->setFormat(IMAGE_TYPE_YUV420, sos.height, sos.width);
	pBstr->fill();
	CImage<float> * imgF = new CImage<float>(pSeq->getFormat());
	CImage<float> * imgLast = new CImage<float>(pSeq->getFormat());
	CImage<int16_t> * img = new CImage<int16_t>(pSeq->getFormat());
	CDynamicHuffman<int16_t> * htree = new CDynamicHuffman<int16_t>();
	CIDCT * idct = new CIDCT();
	CIQuant * iquant = new CIQuant();
	CIZigZag<int16_t, float> * izigzag = new CIZigZag<int16_t, float>();
	CIRLC<int16_t> * irlc = new CIRLC<int16_t>();
	sof_marker_t sof;
	for(uint32_t n = 0 ; n < sos.frames_number; n++)
	{
		dbg("\rDecoding frame: %d", n);
		pBstr->fill();
		pBstr->read_block(&sof, sizeof(sof));
		if(sof.type != MARKER_TYPE_SOF || sof.size != sizeof(sof_marker_t))
		{
			throw utils::StringFormatException("can not sync frame");
		}
		irlc->Decode(pBstr, img);
		izigzag->Transform(img, imgF);
		iquant->Transform(imgF, imgF);
		idct->Transform(imgF, imgF);
		if(sof.frame_type == FRAME_TYPE_I)
		{
			
		}
		else if(sof.frame_type == FRAME_TYPE_P)
		{
			(*imgF) += (*imgLast);
		}
		else
		{
			throw utils::StringFormatException("unknown frame type: 0x%x\n", sof.frame_type);
		}
		pSeq->getFrame() = *imgF;
		if(!pSeq->WriteNext())
		{
			throw utils::StringFormatException("can not write frame to file");
		}
		(*imgLast) = (*imgF);
	}
	dbg("\n");
	delete img;
	delete imgF;
	delete imgLast;
	delete idct;
	delete htree;
	delete izigzag;
	delete iquant;
	delete irlc;
	return false;
}

}
