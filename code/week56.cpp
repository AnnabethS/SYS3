#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "pipeline.hpp"


/////////////////////////////////////////////
// declare function prototypes

void fixRAW();
int getRAWOffset(int row, int cycle, int regNum);
 
/////////////////////////////////////////////
// global variables and class instances
   
   PipelineClass Pipe;

/////////////////////////////////////////////


int main(int argc, char * argv[])
{   
    int c=0;
    
    printf("-- WEEK56.cpp --/n/n");
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
    
    // perform hazard fixes 
        //
        // calls to custom functions here
        //
		fixRAW();
    
    // output resulting pipeline diagram, and test for hazards
       Pipe.DumpPipeline();
       Pipe.PipelineTest();
    
    // end of code     
}

void fixRAW()
{
	for(int i=0; i < Pipe.PipelinedCycles; i++)
	{
		for(int j=0; j < Pipe.OpCount; j++)
		{
			if(Pipe.IsStageRR(j, i))
			{ //we are looking at a read stage
				printf("read_at: cycle %d, instruction %d regnum %d\n", i, j, Pipe.GetRegNum(j, i));
				int x = getRAWOffset(j, i, Pipe.GetRegNum(j, i));
				for(int k=0; k < x; k++)
					Pipe.InsertStall(j, i);
				if(x != 0)
				{
					Pipe.CalculateCycles();
					i = 0;
					j = 0;
				}
				printf("%d\n", x);
			}
		}
	}
}

// returns 0 on no RAW, +ve on the amount of cycles the read must be delayed
int getRAWOffset(int row, int cycle, int regNum)
{
	printf("checking for regnum: %d\n", regNum);
	for(int i=row-1; i >= 0; i--)
	{
		for(int j=cycle; j <= Pipe.PipelinedCycles; j++)
		{
			if(Pipe.IsStageWB(i, j))
			{
				if(Pipe.GetRegNum(i, j) == regNum)
				{
					return (j - cycle) + 1;
				}
			}
		}
	}
	return 0;
}
