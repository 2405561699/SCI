#include "global.h"

#include "Assist.h"


extern vector<Code> arrInstru;
int lines = 1;



void PrintCode(string fileName)
{
	fstream micode(fileName, ios::out);
	for(unsigned int i = 0; i < arrInstru.size(); ++i)
	{
		if(arrInstru[i].m_instru == LIT)
		{
			if(arrInstru[i].m_a)
				micode<<"LIT "<<arrInstru[i].m_a<<endl;
			else
				micode<<"LIT "<<arrInstru[i].m_fvalue<<endl;
		}
		else
		{
			switch (arrInstru[i].m_instru)
			{
				//case LOD:micode<<"LOD "<<arrInstru[i].m_a<<endl;break;
				//case STO:micode<<"STO "<<arrInstru[i].m_a<<endl;break;
				case CAL:micode<<"CAL "<<arrInstru[i].m_a<<endl;break;
				case ALC:micode<<"ALC "<<arrInstru[i].m_a<<endl;break;
				case JMP:micode<<"JMP "<<arrInstru[i].m_a<<endl;break;
				case JPC:micode<<"JPC "<<arrInstru[i].m_a<<endl;break;
				case RET:micode<<"RET "<<arrInstru[i].m_a<<endl;break;
				case WRT:micode<<"WRT "<<arrInstru[i].m_a<<endl;break;
				case IPT:micode<<"IPT "<<arrInstru[i].m_a<<endl;break;
				case FTI:micode<<"FTI "<<arrInstru[i].m_a<<endl;break;
				case ITF:micode<<"ITF "<<arrInstru[i].m_a<<endl;break;
				case OPR:micode<<"OPR "<<arrInstru[i].m_a<<endl;break;
			}
		}


	}
}


void Assist(string fileName)
{
	PrintCode(fileName + ".mic");

}
