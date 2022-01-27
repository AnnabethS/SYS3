#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "pipeline.hpp"


/////////////////////////////////////////////
// declare function prototypes
 
enum stages{
	IF, ID, DF, DS, OF, RR, WB, IALU, FPALU, SHALU, IDLE
};
 
int isStage(int timeStep, int row, stages s);
int getStagesAtStep(int timeStep, stages s);
void findFixError(stages s, int maxConcurrent);

/////////////////////////////////////////////
// global variables and class instances
   
   PipelineClass Pipe;

/////////////////////////////////////////////


int main(int argc, char * argv[])
{   
    int c=0;
    
    printf("-- WEEK34.cpp --/n/n");
    Pipe.StartupMessage();
    
    // configure CPU settings
    
       
       if(1) // single issue
       {
         Pipe.IssueWidth=1;
         Pipe.ReadPorts=2;
         Pipe.WritePorts=1;
         Pipe.IALUCount=1;
         Pipe.FPALUCount=1;
         Pipe.SHALUCount=1;
         Pipe.CacheMode=0;
       }
       else // superscalar 4
       {
         Pipe.IssueWidth=4;
         Pipe.ReadPorts=9;
         Pipe.WritePorts=3;
         Pipe.IALUCount=4;
         Pipe.FPALUCount=1;
         Pipe.SHALUCount=1;
         Pipe.CacheMode=0;
       }
        
    // load in test case and show buffer
    
        c=Pipe.ReadAssemblerCode(argv[1]);
        if(c<0){ return -1; }
        
        Pipe.DumpCodeList();
    
    // generate initial pipeline without and constraints
    
        Pipe.InitialSchedule();
                   
    // output resulting pipeline diagram, and test for hazards
        Pipe.DumpPipeline();
        Pipe.PipelineTest();

		for(int i=0; i < Pipe.PipelinedCycles; i++)
		{
			int x = getStagesAtStep(i, IALU);
			if(x > Pipe.IALUCount)
			{
				// we have found an error
				// find the 2nd IALU usage and add a stall
				int IALUs = 0;
				for(int j=0; j < Pipe.OpCount; j++)
				{
					if(isStage(i, j, IALU))
					{
						IALUs++;
					}
					if(IALUs > Pipe.IALUCount)
					{
						printf("row: %d col: %d\n", j, i);
						Pipe.InsertStall(j, i);
						Pipe.DumpPipeline();
						Pipe.CalculateCycles();
						i = 0;
						break;
					}
				}
			}
		}

    
    // perform hazard fixes 
        //
        // calls to custom functions here
        //

		findFixError(IALU, Pipe.IALUCount);
		findFixError(RR, Pipe.ReadPorts);
		
    
    // output resulting pipeline diagram, and test for hazards
	Pipe.DumpPipeline();
	Pipe.PipelineTest();
    
    // end of code     
}

int getStagesAtStep(int timeStep, stages s)
{
	int stageAmount = 0;
	for(int j=0; j < MAXINSTRUCTIONS; j++)
	{
		if(isStage(timeStep, j, s))
		{
			stageAmount++;
		}
	}
	return stageAmount;
}

int isStage(int timeStep, int row, stages s)
{
	switch(s)
	{
	case IF:
		return Pipe.IsStageIF(row, timeStep);
	case ID:
		return Pipe.IsStageID(row, timeStep);
	case DF:
		return Pipe.IsStageDF(row, timeStep);
	case DS:
		return Pipe.IsStageDS(row, timeStep);
	case OF:
		return Pipe.IsStageOF(row, timeStep);
	case RR:
		return Pipe.IsStageRR(row, timeStep);
	case WB:
		return Pipe.IsStageWB(row, timeStep);
	case IALU:
		return Pipe.IsStageIALU(row, timeStep);
	case FPALU:
		return Pipe.IsStageFPALU(row, timeStep);
	case SHALU:
		return Pipe.IsStageSHALU(row, timeStep);
	case IDLE:
		return Pipe.IsStageIDLE(row, timeStep);
	}
}


void findFixError(stages s, int maxConcurrent)
{
	for(int i=0; i < Pipe.PipelinedCycles; i++)
	{
		int x = getStagesAtStep(i, s);
		if(x > maxConcurrent)
		{
			// we have found an error
			// find the 2nd IALU usage and add a stall
			int IALUs = 0;
			for(int j=0; j < Pipe.OpCount; j++)
			{
				if(isStage(i, j, s))
				{
					IALUs++;
				}
				if(IALUs > maxConcurrent)
				{
					printf("row: %d col: %d\n", j, i);
					Pipe.InsertStall(j, i);
					Pipe.CalculateCycles();
					i = 0;
					break;
				}
			}
		}
	}
}
