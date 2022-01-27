#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "pipeline.hpp"


/////////////////////////////////////////////
// declare function prototypes
   
   void FixWeek2Example();
 
/////////////////////////////////////////////
// global variables and class instances
   
   PipelineClass Pipe;

/////////////////////////////////////////////


int main(int argc, char * argv[])
{   
    int c=0;
    
    printf("-- WEEK2.cpp --/n/n");
    Pipe.StartupMessage();
    
    // configure CPU settings
        Pipe.ReadPorts=3;
        Pipe.WritePorts=1;
        Pipe.IssueWidth=1;
        Pipe.IALUCount=2;
        Pipe.FPALUCount=2;
        Pipe.SHALUCount=2;
        Pipe.CacheMode=0;
    
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
        // FixWeek2Example();
        //
    
    // output resulting pipeline diagram, and test for hazards
        //Pipe.DumpPipeline();
        //Pipe.PipelineTest();
    
    // end of code     
}

void FixWeek2Example()
{
    Pipe.InsertStall(1,3);
    Pipe.InsertStall(1,3);
    
    Pipe.InsertStall(5,7);
    Pipe.InsertStall(5,7);
    Pipe.InsertStall(5,7);
    
    Pipe.InsertStall(4,9);
}
