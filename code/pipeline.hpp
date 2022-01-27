#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>



#define MAXSTAGES 200
#define MAXINSTRUCTIONS 20

#define TABLE_1PORTMEMORY  1

class PipelineClass 
{       
 public:             
    
    void StartupMessage(void);
    int  ReadAssemblerCode(char *FileName);
    void GetTerm(char *s, char *t, int n);
    
    
    
    void DumpCodeList();
    void DumpPipeline();
    void InitialSchedule();
    void PipelineTest();
    
    void Init();
    void ResetTable();
    
    int IsStageIF(int i, int c);
    int IsStageID(int i, int c);
    int IsStageDF(int i, int c);
    int IsStageDS(int i, int c);
    int IsStageOF(int i, int c);
    int IsStageRR(int i, int c);
    int IsStageWB(int i, int c);
    int IsStageIALU(int i, int c);
    int IsStageFPALU(int i, int c);
    int IsStageSHALU(int i, int c);
    int IsStageIDLE(int i, int c);
    
    int GetRegNum(int instr, int cyc);
    
    void InsertStage(int i, int c, char *s);
    void InsertStall(int i, int c);
    void DeleteStage(int i, int c);
    
    void InsertOp(int i,char * s1,char*s2,char*s3,char*s4);
    void DeleteOp(int i);
    void SwapOps(int i, int j);
    void CalculateCycles();
           
    int OpCount;
    
 //private:
    
    void ConvertString(char *);
    int IsStageXX(int i, int c, char * s);
    
   
    
    char Opcode[1024][8];
    char  OperX[1024][8];
    char  OperY[1024][8];
    char  OperZ[1024][8];
    
    char Stage[MAXINSTRUCTIONS][MAXSTAGES][8];
    
    int Table[20][MAXSTAGES];
    
    
    int ReadPorts=3;
    int WritePorts=1;
    int IssueWidth=1;
    
    int IALUCount=2;
    int FPALUCount=2;
    int SHALUCount=2;
    
    int CacheMode=0;
    
    int SequentialCycles=0;
    int PipelinedCycles=0;
    
};
    
