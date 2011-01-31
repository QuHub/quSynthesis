#pragma once
// BestCost         3261193(M=3)  3398744(M=2)      3407896       3398961
#define N_POP 519 // 100             100           200
#define N_GEN 100 // 200             300           500
#define N_RUN 4   // 10              10            20
#define PM    0.3 // 0.3             0.15          0.1
#define PC    0.7 // 0.7             0.6           0.6
//#define TENURE	30
//#define LONGMEM 100

#include <algorithm>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>

#define CROSS_OVER SinglePointCrossOver
//#define CROSS_OVER TwoPointCrossOver

namespace QuLogic 
{
	
  class GAConductor: QuConductor {

  private:
    int m_BestFit;
    double m_ParentTotalFitness;
    gcroot<Type^> m_AlgoType;
	map<int,int> taboo_table;
	vector<ULONGLONG> taboo_list;
	int nBands;
	int nNeighbors;
	int Tenure;
	int LongMem;
	int Portion;
	int maxBand;
	bool Aspire;
  public:
    QuAlgorithm *m_pAlgo[N_POP*2];
	QuAlgorithm *current; //taboo choosen current point

    ~GAConductor()
    {
      for (int i=0; i<N_POP; i++)
        delete m_pAlgo[i];
    }

    GAConductor(int nBits, Type^ T) : QuConductor(nBits)
    {
      m_AlgoType = T;
      ZeroMemory(m_pAlgo, sizeof(PVOID) * 2 * N_POP);
	  nNeighbors=519;
	  Tenure=20;
	  LongMem =500;
	  Portion =4;
	  srand ( time(NULL) );
    }

    void InitPopulation()
    {
		Rand::NextInteger(10);
		Rand::NextInteger(10);
      // Setup population of individuals randomly
	nBands=(int)Math::Pow(2,M) * (m_nBits-M+1) ; //needed to determine  moves in taboo
	
	Console::WriteLine("nBands {0}", nBands);
      for (int i=0; i<nNeighbors; i++) 
	  {
        // STEP(2): Add your new algorithm here...
        if(m_AlgoType->Name == "CoveredSetPartition") 
			{
				m_pAlgo[i] = new CoveredSetPartition(m_nBits);
				
		     }

	  }
	  maxBand =0;
	  for(int band=0;band<(nBands-1);band++) 
	   {
		int len= QuLogic::BandBoundary[band+1]-QuLogic::BandBoundary[band];
		if(len> maxBand) maxBand= len;
	    }

    }

    void Synthesize(PULONGLONG pOut)
    {
      
      // Calculate Cost Function for all individuals
      for (int r=0; r<N_RUN; r++) {
		  taboo_list.clear();
		  taboo_table.clear();
		  m_BestFit = MAXLONGLONG;
		  Console::WriteLine("Run: {0} portion:{1}", r,Portion);
		  ofstream log;
		  log.open ("tabu_log.txt",ios::app);
		  log << "taboo\nportion:"<<Portion<<"\tTenure:"<<Tenure<<"\tlong mem:"<<LongMem<<"\n";
		  log.flush();
		  log.close();

        InitPopulation();
        for (int g=0; g<N_GEN; g++) {
          DoGeneration(g, pOut);
        }
		Tenure+=5;
      }
	  Console::ReadLine();
    }

    void DoGeneration(int gen, PULONGLONG pOut)
    {
      m_ParentTotalFitness = 0;
      for (int i=0; i<nNeighbors; i++) {
        m_pAlgo[i]->Synthesize(pOut);
      }

      WaitForQueue();
	  Aspire= false;
      for (int i=0; i<nNeighbors; i++) {
        ULONGLONG qCost = m_pAlgo[i]->m_QuantumCost;
        m_ParentTotalFitness += qCost;
        if (m_BestFit > qCost) {
          m_BestFit = qCost;
		  Aspire =true;
		
        }
      }
	  Console::WriteLine("{0} ;{1}", gen, m_BestFit);          		
		ofstream log;
		log.open ("tabu_log.txt",ios::app);
		log << gen<<';'<<m_BestFit<<"\n";
		log.close();


//      Breed();
	 Taboo_Breed(gen);
      Cull();
    }

    void Cull()
    {
      for (int i=0; i<nNeighbors; i++) {
        delete m_pAlgo[i];
        m_pAlgo[i] = m_pAlgo[i+nNeighbors];
      }
    }

    void Breed()
    {
	
      for (int i=0; i<N_POP; i++) {
        QuAlgorithm *p1 = Roulette();  
        QuAlgorithm *p2 = Roulette();
        m_pAlgo[i+N_POP] = p1->CROSS_OVER(p2, PC);
        m_pAlgo[i+N_POP]->Mutate(PM);
      }
	}

	string  InputCodeS(PULONGLONG pIn,int bits)
	{
	   ostringstream result;
		for(int x=0;x<(1<<bits);x++)
		{
			//o=o<<bits|pIn[x];	
			result << pIn[x];
//			Console::WriteLine("current solution {0} ", pIn[x]);
		}
		return result.str();
	}
	ULONGLONG  InputCode(PULONGLONG pIn,int bits)
	{
	   ULONGLONG result;
		for(int x=0;x<(1<<bits);x++)
		{
			//o=o<<bits|pIn[x];	
			result+= x*pIn[x];
//			Console::WriteLine("current solution {0} ", pIn[x]);
		}
		return result;
	}
	void Taboo_Breed(int gen)//Baker : I will change fto make taboo
    {
		// 1- find the best solution in the population
		// 2- if not taboo use it to generare the next popuation
		// 3- else  if it is aspiration use it or use the with the next one goto 2
		//4- generating the next generation by using (**MOVE**)

		//for (int i=0; i<N_POP; i++) {
		//for all bands
		// select current move

	//	sort(m_pAlgo,m_pAlgo+N_POP);
		for(int i=0;i<(nNeighbors-1);i++)
			for(int j=i+1;j<nNeighbors;j++)
				if(m_pAlgo[i]->m_QuantumCost>m_pAlgo[j]->m_QuantumCost)
				{
					QuAlgorithm* temp =m_pAlgo[i];
								m_pAlgo[i]=m_pAlgo[j];
								m_pAlgo[j]=temp;
				}
		int c=0;

		//if (m_pAlgo[0]->m_QuantumCost==m_BestFit) //aspiration (the best so far)
		if (Aspire)
		{
			current= m_pAlgo[0];
			Console::WriteLine("aspiration");
		}
		else //check taboo moves
			{ 
				c=0;
				current= m_pAlgo[c];
				//coding the input order
				ULONGLONG o =InputCode(m_pAlgo[c]->m_pIn,m_pAlgo[c]->m_nBits);
				while((gen < taboo_table[current->m_i])||std::find(taboo_list.begin(), taboo_list.end(), o)!=taboo_list.end())
				{
					c++;
					current= m_pAlgo[c];
					o =InputCode(m_pAlgo[c]->m_pIn,m_pAlgo[c]->m_nBits);
//					Console::WriteLine("inside while Tenure {0} : band {1} : cost {2} ", taboo_table[current->m_i],current->m_i,m_pAlgo[c]->m_QuantumCost);
					//cout << o;
				}
			}
		//update memory
		//taboo_table[current->m_i]=gen +Tenure;
		// dinymic tenure;
		int band_length = QuLogic::BandBoundary[current->m_i+1]-QuLogic::BandBoundary[current->m_i];
		taboo_table[current->m_i]=gen +(maxBand-band_length)/2+Tenure;
		Console::WriteLine("Tenure {0} : band {1} :order {2}",(maxBand-band_length)/2+Tenure,current->m_i,c);

		//long term mermory
				//coding the input order
		ULONGLONG o =InputCode(m_pAlgo[c]->m_pIn,m_pAlgo[c]->m_nBits);
//		Console::WriteLine("current solution {0} ", o);
		taboo_list.push_back(o);
		//forget old solutions
		if(taboo_list.size()>LongMem) taboo_list.erase(taboo_list.begin());
		/// generate new neighbourhood (generation)
int		k=0;
		for(int band=0;band<(nBands-1);band++)
		{	
			int nStart= QuLogic::BandBoundary[band];
			int nEnd = QuLogic::BandBoundary[band+1];
			// to be proportional to the the band size
			int x=(nEnd-nStart)/Portion;
			//if( x==0) x=1;
			//int x=1;
			for(int i=0;i<x;i++)
			{
			
				QuAlgorithm *p = m_pAlgo[c]->Copy();
				p->Move(band);
				m_pAlgo[k+nNeighbors]=p;
				m_pAlgo[k+nNeighbors]->m_i =band;
				k++;
			}
			
			/*
			for(int nFirst=nStart;nFirst<(nEnd-1);nFirst++)
				for(int nSecond=nFirst+1;nSecond<nEnd;nSecond++)
				{
					//i++; 
					QuAlgorithm *p = current->Copy();
					p->Move(nFirst,nSecond);
					m_pAlgo[i+N_POP]=p;
					m_pAlgo[i+N_POP]->m_i =i;
				}
				*/
		}//all bands
		nNeighbors=k;
      }



    QuAlgorithm *Roulette()
    {
      double rnd = Rand::NextDouble();
      double val=0;

      for (int i=0; i < N_POP; i++) {
        val += m_pAlgo[i]->m_QuantumCost/m_ParentTotalFitness;
        if (rnd < val)
          return m_pAlgo[i];
      }

      return m_pAlgo[N_POP-1];
    }
  };


}

