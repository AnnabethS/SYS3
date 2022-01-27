
#include "pipeline.hpp"

/////// FUNCTIONS ////////////

void PipelineClass::Init()
{
        memset(&Opcode,0,sizeof(Opcode));
        memset(&OperX,0,sizeof(OperX));
        memset(&OperY,0,sizeof(OperY));
        memset(&OperZ,0,sizeof(OperZ));
     
        memset(&Stage,0,sizeof(Stage));
        memset(&Table,0,sizeof(Table));
        
        OpCount=0;
        
}

void PipelineClass::ResetTable()
{
   memset(&Table,0,sizeof(Table));
}

void PipelineClass::StartupMessage(void)
{

  printf("\n");
  printf("-----------------------------------------------\n");
  printf("----- PIPELINE MICROASSEMBLER v  2021.100 -----\n");
  printf("---- (c) 2021 C Crispin-Bailey Uni of York ----\n");
  printf("-----------------------------------------------\n");
  printf("\n");

}

int PipelineClass::ReadAssemblerCode(char * FileName)
{
    FILE * ip;
    char line[1024];
    char term[1024];
    
        OpCount=0;
        
        ip = fopen(FileName,"r");
        if (ip==NULL) { printf("file error, File: %s not found\n\n",FileName); return(-1); }
        
        printf("---- READING FILE ----\n");
        
        
        while(feof(ip)==0)
        {
            fgets(line,1023,ip);
            if(line[0]=='/'){ continue; }
            if(feof(ip)!=0){continue;}
            if(strncmp(line,"END",strlen("END"))==0){ continue;}
            
            ConvertString(line);
            
    
            printf("[%s] : ",line);
            
            GetTerm(line,Opcode[OpCount],1);
            GetTerm(line, OperX[OpCount],2); 
            GetTerm(line, OperY[OpCount],3); 
            GetTerm(line, OperZ[OpCount],4); 
            
            
            printf("%d: %s - %s - %s - %s\n",OpCount
              ,Opcode[OpCount],OperX[OpCount]
              ,OperY[OpCount],OperZ[OpCount]  );
              
            OpCount++;   
            if(OpCount > MAXINSTRUCTIONS){ break;}
        }
        
        fclose(ip);
    return(1);
}

void PipelineClass::DumpCodeList()
{
    printf("\n---- ASM CODE LISTING ----------\n");
    
    for(int i=0;i<OpCount;i++)
    {
       printf("%d: %s - %s - %s - %s\n",i
              ,Opcode[i],OperX[i]
              ,OperY[i],OperZ[i]  );
    }
}

void PipelineClass::InsertOp(int i,char * s1,char*s2,char*s3,char*s4)
{

        for(int x=OpCount;x>i;x--)
        {
           strcpy(Opcode[x],Opcode[x-1]);
           strcpy( OperX[x],OperX[x-1]);
           strcpy( OperY[x],OperY[x-1]);
           strcpy( OperZ[x],OperZ[x-1]);           
        }
        
        strcpy(Opcode[i],s1);
        strcpy(OperX[i] ,s2);
        strcpy(OperY[i] ,s3);
        strcpy(OperZ[i] ,s4);    
}

void PipelineClass::DeleteOp(int i)
{
        for(int x=i;x<OpCount;x++)
        {
            strcpy(Opcode[x],Opcode[x+1]);
            strcpy(OperX[x],OperX[x+1]);
            strcpy(OperY[x],OperY[x+1]);
            strcpy(OperZ[x],OperZ[x+1]);           
        }
}

void PipelineClass::SwapOps(int i, int j)
{
    char Tmp[1024];
    
    
      strcpy(Tmp,Opcode[i]);  strcpy(Opcode[i],Opcode[j]); strcpy(Opcode[j],Tmp);
      strcpy(Tmp,OperX[i] );  strcpy(OperX[i] ,OperX[j] ); strcpy(OperX[j],Tmp );
      strcpy(Tmp,OperY[i] );  strcpy(OperY[i] ,OperY[j] ); strcpy(OperY[j],Tmp );
      strcpy(Tmp,OperZ[i] );  strcpy(OperZ[i] ,OperZ[j] ); strcpy(OperZ[j],Tmp );
             

}

void PipelineClass::ConvertString(char *s)
{
    //// converts string to csv formatted sequence ////
    
    for(int i=0;i<strlen(s);i++)
    {
        if( s[i]==0)    {         continue;}
        
        if( s[i]=='\r') { s[i]=0; continue; }
        if (s[i]<=32)   { s[i]=',';         }
    }
}

void PipelineClass::GetTerm(char *s, char *t, int n)
{
    int tcount=0;
    int start=0;
      
    strcpy(t,"");
    
    for(int i = 0;i<strlen(s);i++)
    {
        if( s[i]==',')
        { tcount++; 
        
            if(tcount==n)
             {
                strcpy(t,s+start);
            
                if(strstr(t,",")!=NULL) {  *(strstr(t,","))  =0;}
             }
        
            start=i+1;
        }
    }
    
}

int PipelineClass::IsStageIF(int instr, int cyc)
{ return IsStageXX(instr,cyc,"IF"); }

int PipelineClass::IsStageID(int instr, int cyc)
{ return IsStageXX(instr,cyc,"ID"); }

int PipelineClass::IsStageDF(int instr, int cyc)
{ return IsStageXX(instr,cyc,"DF"); }

int PipelineClass::IsStageDS(int instr, int cyc)
{ return IsStageXX(instr,cyc,"DS"); }


int PipelineClass::IsStageOF(int instr, int cyc)
{ return IsStageXX(instr,cyc,"OF"); }

int PipelineClass::IsStageRR(int instr, int cyc)
{ return IsStageXX(instr,cyc,"RR"); }

int PipelineClass::IsStageWB(int instr, int cyc)
{ return IsStageXX(instr,cyc,"WB"); }

int PipelineClass::IsStageIALU(int instr, int cyc)
{ return IsStageXX(instr,cyc,"IA"); }

int PipelineClass::IsStageFPALU(int instr, int cyc)
{ return IsStageXX(instr,cyc,"FA"); }

int PipelineClass::IsStageSHALU(int instr, int cyc)
{ return IsStageXX(instr,cyc,"SA"); }

int PipelineClass::IsStageIDLE(int instr, int cyc)
{ 
   if(Stage[instr][cyc][0]==0) { return 0;} else { return 1;}
}


int PipelineClass::IsStageXX(int instr, int cyc, char * str)
{
    if(strstr(Stage[instr][cyc],str)==NULL) { return 0; } else { return 1;}  
}


int PipelineClass::GetRegNum(int instr, int cyc)
{
       
    if(strncmp("RR..R",Stage[instr][cyc],5)==0) { return atoi(&Stage[instr][cyc][5]); }
    if(strncmp("WB..R",Stage[instr][cyc],5)==0) { return atoi(&Stage[instr][cyc][5]); }
    
    
    return( -1 );
    
}

void PipelineClass::CalculateCycles()
{
    PipelinedCycles=0;
    
    for(int c=0;c<MAXSTAGES;c++)
    {
        for(int i=0;i<OpCount;i++)
        {
            if(IsStageIDLE(i,c)!=0)
            {
                PipelinedCycles=c;
            }
        }
    }
    
    SequentialCycles=0;
    
    for(int c=0;c<MAXSTAGES;c++)
    {
        for(int i=0;i<OpCount;i++)
        {
        //printf("%s.",Stage[i][c]);
        
            if(Stage[i][c][0]==0)
            {}//printf("<0>");}
            else 
            if(IsStageXX(i,c,"[  --  ]"))
            {}//printf("<-->");}
            else
            if(IsStageXX(i,c,"------"))
            {}//printf("<------>");}
            else
            
            { //printf("<%s>",Stage[i][c]);
            SequentialCycles++; }
        }
    }
    

}

void PipelineClass::InitialSchedule()
{
    int Ppos=0;
    int p=0;
    
    for(int i = 0; i<OpCount; i++)  // instruction range
    {
    
        
        if(strncmp(Opcode[i],"LDI",strlen("LDI"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            sprintf(Stage[i][Ppos+2],"--OF--");
            sprintf(Stage[i][Ppos+3],"WB..%s",OperX[i]);
            
                     
        }
        else
         if(strncmp(Opcode[i],"STIX",strlen("STIX"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            sprintf(Stage[i][Ppos+2],"RR..%s",OperX[i]);
            sprintf(Stage[i][Ppos+3],"RR..%s",OperY[i]);
            sprintf(Stage[i][Ppos+4],"--IA--");
            sprintf(Stage[i][Ppos+5],"RR..%s",OperZ[i]); 
            sprintf(Stage[i][Ppos+6],"--DS--"); 
            
        }
        else
        if(strncmp(Opcode[i],"IADD",strlen("IADD"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            sprintf(Stage[i][Ppos+2],"RR..%s",OperX[i]);
            sprintf(Stage[i][Ppos+3],"RR..%s",OperY[i]);
            sprintf(Stage[i][Ppos+4],"--IA--");
            sprintf(Stage[i][Ppos+5],"WB..%s",OperZ[i]);       
        }
        else
        if(strncmp(Opcode[i],"IMUL",strlen("IMUL"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            sprintf(Stage[i][Ppos+2],"RR..%s",OperX[i]);
            sprintf(Stage[i][Ppos+3],"RR..%s",OperY[i]);
            sprintf(Stage[i][Ppos+4],"--IA--");
            sprintf(Stage[i][Ppos+5],"--IA--");
            sprintf(Stage[i][Ppos+6],"WB..%s",OperZ[i]);       
        }
        else
        if(strncmp(Opcode[i],"IDIV",strlen("IDIV"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            sprintf(Stage[i][Ppos+2],"RR..%s",OperX[i]);
            sprintf(Stage[i][Ppos+3],"RR..%s",OperY[i]);
            sprintf(Stage[i][Ppos+4],"--IA--");
            sprintf(Stage[i][Ppos+5],"--IA--");
            sprintf(Stage[i][Ppos+6],"--IA--");
            sprintf(Stage[i][Ppos+7],"WB..%s",OperZ[i]);       
        }
        else
        if(strncmp(Opcode[i],"FADD",strlen("FADD"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            sprintf(Stage[i][Ppos+2],"RR..%s",OperX[i]);
            sprintf(Stage[i][Ppos+3],"RR..%s",OperY[i]);
            sprintf(Stage[i][Ppos+4],"--FA--");
            sprintf(Stage[i][Ppos+5],"--FA--");
            sprintf(Stage[i][Ppos+6],"WB..%s",OperZ[i]);        
        }
        else
        if(strncmp(Opcode[i],"FMUL",strlen("FMUL"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            sprintf(Stage[i][Ppos+2],"RR..%s",OperX[i]);
            sprintf(Stage[i][Ppos+3],"RR..%s",OperY[i]);
            sprintf(Stage[i][Ppos+4],"--FA--");
            sprintf(Stage[i][Ppos+5],"--FA--");
            sprintf(Stage[i][Ppos+6],"--FA--");
            sprintf(Stage[i][Ppos+7],"WB..%s",OperZ[i]);        
        }
        else
        if(strncmp(Opcode[i],"FDIV",strlen("FDIV"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            sprintf(Stage[i][Ppos+2],"RR..%s",OperX[i]);
            sprintf(Stage[i][Ppos+3],"RR..%s",OperY[i]);
            sprintf(Stage[i][Ppos+4],"--FA--");
            sprintf(Stage[i][Ppos+5],"--FA--");
            sprintf(Stage[i][Ppos+6],"--FA--");
            sprintf(Stage[i][Ppos+7],"--FA--");
            sprintf(Stage[i][Ppos+8],"--FA--");
            sprintf(Stage[i][Ppos+9],"WB..%s",OperZ[i]);        
        }
        else
        if(strncmp(Opcode[i],"SHR",strlen("SHR"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            sprintf(Stage[i][Ppos+2],"RR..%s",OperX[i]);
            sprintf(Stage[i][Ppos+3],"--SA--");
            sprintf(Stage[i][Ppos+4],"WB..%s",OperX[i]);        
        }
        else
        if(strncmp(Opcode[i],"SHL",strlen("SHL"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            sprintf(Stage[i][Ppos+2],"RR..%s",OperX[i]);
            sprintf(Stage[i][Ppos+3],"--SA--");
            sprintf(Stage[i][Ppos+4],"WB..%s",OperX[i]);        
        }
        else
        if(strncmp(Opcode[i],"TSTR",strlen("TSTR"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            sprintf(Stage[i][Ppos+2],"RR..%s",OperX[i]);
            sprintf(Stage[i][Ppos+3],"RR..%s",OperY[i]);
            sprintf(Stage[i][Ppos+4],"--IA--");
            sprintf(Stage[i][Ppos+5],"RR..%s",OperX[i]);
            sprintf(Stage[i][Ppos+6],"RR..%s",OperZ[i]);
            sprintf(Stage[i][Ppos+7],"--IA--");
            sprintf(Stage[i][Ppos+8],"WB..%s",OperX[i]); 
        }

        else
        if(strncmp(Opcode[i],"LDR",strlen("LDR"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            sprintf(Stage[i][Ppos+2],"RR..%s",OperX[i]);
            sprintf(Stage[i][Ppos+3],"--OF--");
            sprintf(Stage[i][Ppos+4],"WB..%s",OperY[i]);   
        }
        else
        if(strncmp(Opcode[i],"STR",strlen("STR"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            sprintf(Stage[i][Ppos+2],"RR..%s",OperX[i]);
            sprintf(Stage[i][Ppos+3],"RR..%s",OperY[i]);
            sprintf(Stage[i][Ppos+4],"--DS--");
        }
        else
        if(strncmp(Opcode[i],"LDM",strlen("LDM"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            sprintf(Stage[i][Ppos+2],"--OF--");
            sprintf(Stage[i][Ppos+3],"--DF--");
            sprintf(Stage[i][Ppos+4],"WB..%s",OperX[i]);      
            
              
        }
         else
        if(strncmp(Opcode[i],"INC",strlen("INC"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            sprintf(Stage[i][Ppos+2],"RR..%s",OperX[i]);
            sprintf(Stage[i][Ppos+3],"--IA--");
            sprintf(Stage[i][Ppos+4],"WB..%s",OperX[i]);        
              
        }
           else
        if(strncmp(Opcode[i],"DEC",strlen("DEC"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            sprintf(Stage[i][Ppos+2],"RR..%s",OperX[i]);
            sprintf(Stage[i][Ppos+3],"--IA--");
            sprintf(Stage[i][Ppos+4],"WB..%s",OperX[i]);        
              
        }

        else
        if(strncmp(Opcode[i],"NOP",strlen("NOP"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            
                                        
        }
        else
        if(strncmp(Opcode[i],"MAC",strlen("MAC"))==0)
        {
            sprintf(Stage[i][Ppos+0],"--IF--");
            sprintf(Stage[i][Ppos+1],"--ID--");
            sprintf(Stage[i][Ppos+2],"RR..%s",OperX[i]);
            sprintf(Stage[i][Ppos+3],"--OF--");
            sprintf(Stage[i][Ppos+4],"--FA--");
            sprintf(Stage[i][Ppos+4],"--FA--");
            sprintf(Stage[i][Ppos+5],"RR..%s",OperY[i]);
            sprintf(Stage[i][Ppos+6],"--FA--");
            sprintf(Stage[i][Ppos+7],"WB..%s",OperY[i]);
             
        }

        
        else {  } //{Ppos--;} 
        
        p++; if(p%IssueWidth==0){ Ppos++; }
    
        
        
    }
    
}

void PipelineClass::PipelineTest()
{
    int flag=0;
    int MCount,ICount,DCount;
    int i,c;
    
        // reset table content
           memset(&Table,0,sizeof(Table));
     
     
        /////////////////////////////////////////////////
        // DATA REPORT
     
            CalculateCycles();
            
            printf("\n");
            printf("-- [ STATS ] -------------------------------------------\n");
            printf("\n");

            printf(" Instruction Count % 4d\n",OpCount);
            printf(" Sequential Cycles % 4d\n",SequentialCycles);
            printf(" Pipelined  Cycles % 4d\n",PipelinedCycles);
            
            if(PipelinedCycles>0)
            {
              printf(" Speedup          %5.2f\n",(float)SequentialCycles/(float)PipelinedCycles);
            }
            
            if(SequentialCycles>0)
            {
                printf(" Sequential CPI      %5.2f\n",(float)SequentialCycles/(float)OpCount);
            }
            
            if(PipelinedCycles>0)
            {
                printf(" Pipelined  CPI      %5.2f\n",(float)PipelinedCycles/(float)OpCount);
            }
            
        /////////////////////////////////////////////////
        // test for Single Port Memory Conflicts
        
        if(CacheMode==0)
        {
           flag=0;
          
           printf("\n-- [ MEMORY HAZARDS (Unified Cache) ] ----------------\n");
           printf("\n");

           
           for( c=0;c<MAXSTAGES;c++)
           {
             MCount=0;
                          
             for( i=0;i<OpCount; i++)
             {  
               MCount += IsStageIF(i,c)+IsStageDF(i,c)+IsStageOF(i,c)+IsStageDS(i,c);
                
                Table[TABLE_1PORTMEMORY][c] += IsStageIF(i,c)+IsStageDF(i,c)+IsStageOF(i,c)+IsStageDS(i,c);
                
                if(MCount>1) 
                { printf(" Memory Hazard(s) at Instruction %d Cycle %d [%s]\n"
                   ,i,c,Stage[i][c]); 
                   
                   MCount=1; // decrease to force a new selective detection event
                }
             }
           }
          }
          
        /////////////////////////////////////////////////
        // test for Split Cache Port Memory Conflicts
        
        if(CacheMode==1)
        {
           flag=0;
          
           printf("\n-- [ MEMORY HAZARDS (Split Cache)] ----------------\n");
           printf("\n");


           for( c=0;c<MAXSTAGES;c++)
           {
             MCount=0;
             ICount=0;
             DCount=0;
             
             for( i=0;i<OpCount; i++)
             {  
                ICount += IsStageIF(i,c)+IsStageOF(i,c);
                DCount += IsStageDF(i,c)+IsStageDS(i,c);
               
                Table[TABLE_1PORTMEMORY][c] += IsStageIF(i,c)+IsStageDF(i,c)+IsStageOF(i,c);
                
                if(ICount>1) 
                { printf(" Memory Hazard I-CACHE at Instruction %d Cycle %d [%s]\n"
                   ,i,c,Stage[i][c]); 
                   ICount--; // decrease to force a new selective detection event
                }
                
                if(DCount>1) 
                { printf(" Memory Hazard D-CACHE at Instruction %d Cycle %d [%s]\n"
                   ,i,c,Stage[i][c]); 
                   DCount--; // decrease to force a new selective detection event
                }
             }
           }
        }





           printf("\n-- [ REGISTER HAZARDS ] ----------------\n");
           printf("\n");
           

        //////////////////////////////////////
        // Test for Read After Write RAW 
        
            // Find every read and check if there is a preceding/coincident 
            // write on a later row
            
            int R1,R2;
            
            
            
            for(i=0;i<OpCount;i++)
            {
                for(c=0;c<MAXSTAGES;c++)
                {
                
                  if(IsStageRR(i,c))
                  {
                    //printf("RR at stage %d of op %d\n", c,i);
                    
                    // get RegNum
                    
                        R1 = GetRegNum(i,c);
                        
                    //Search preceding instructions to see if preceding writes 
                    //are arriving late
                    
                    if(i>0)
                    for(int ix=0;ix<i;ix++)
                    { for(int cx=c; cx<MAXSTAGES; cx++)
                        { if(IsStageWB(ix,cx))
                            { R2=GetRegNum(ix,cx);
                              
                              if(R1==R2)
                              {
                                printf(" RAW Hazard ? --  RR.%d(i%d,c%d)) WB.%d,(i%d,c%d)\n"
                                ,R1,i,c,R2,ix,cx);
                              }
                            }
                        }
                     }
                     
                  }
                }
           }
           
        //////////////////////////////////////
        // Test for Read After Write RAW 
        
            // Find every read and check if there is a preceding/coincident 
            // write on a later row
              
            
            for(i=0;i<OpCount;i++)
            {
                for(c=0;c<MAXSTAGES;c++)
                {
                
                  if(IsStageWB(i,c))
                  {
                    //printf("RR at stage %d of op %d\n", c,i);
                    
                    // get RegNum
                    
                        R1 = GetRegNum(i,c);
                        
                    //Search preceding instructions to see if preceding writes 
                    //are arriving late
                    
                    if(i>0)
                    for(int ix=0;ix<i;ix++)
                    { for(int cx=c; cx<MAXSTAGES; cx++)
                        { if(IsStageRR(ix,cx))
                            { R2=GetRegNum(ix,cx);
                              
                              if(R1==R2)
                              {
                                printf(" WAR Hazard ? --  RR.%d(i%d,c%d)) WB.%d,(i%d,c%d)\n"
                                ,R1,i,c,R2,ix,cx);
                              }
                            }
                        }
                     }
                     
                  }
                }
           }


       //////////////////////////////////////
        // Test for Write After Write RAW 
        
            // Find every read and check if there is a preceding/coincident 
            // write on a later row
              
            
            for(i=0;i<OpCount;i++)
            {
                for(c=0;c<MAXSTAGES;c++)
                {
                
                  if(IsStageWB(i,c))
                  {
                    //printf("WB at stage %d of op %d\n", c,i);
                    
                    // get RegNum
                    
                        R1 = GetRegNum(i,c);
                        
                    //Search preceding instructions to see if preceding writes 
                    //are arriving late
                    
                    if(i>0)
                    for(int ix=0;ix<i;ix++)
                    { for(int cx=c; cx<MAXSTAGES; cx++)
                        { if(IsStageWB(ix,cx))
                            { R2=GetRegNum(ix,cx);
                              
                              if(R1==R2)
                              {
                                printf(" WAW Hazard ? --  WB.%d(i%d,c%d)) WB.%d,(i%d,c%d)\n"
                                ,R1,i,c,R2,ix,cx);
                              }
                            }
                        }
                     }
                     
                  }
                }
           }


        printf("\n-- [ STRUCTURAL HAZARDS ] ----------------\n");
        printf("\n");


        /////////////////////////////////////////
        // test read write port utilisation conflicts
        
     
           int IALU=0;
           int FPALU=0;
           int SHALU=0;
           
            
                for(c=0;c<MAXSTAGES;c++)
                {
                                        IALU=0;
                    FPALU=0;
                    SHALU=0;
                    
                     for(i=0;i<OpCount;i++)
                     {
                                              
                        if(IsStageIALU(i,c)){ IALU++;}
                        if(IsStageFPALU(i,c)){ FPALU++;}
                        if(IsStageSHALU(i,c)){ SHALU++;}
                        
                     }
                     
                     
                                         
                     if(IALU>IALUCount) 
                     { printf(" IALU Utility Exceeded at c%02d (%d used vs %d available)\n",c,IALU,IALUCount);}
                    
                     if(FPALU>FPALUCount) 
                     { printf(" FPALU Utility Exceeded at c%02d (%d used vs %d available)\n",c,FPALU,FPALUCount);}
                     
                     if(SHALU>FPALUCount) 
                     { printf(" SHALU Utility Exceeded at c%02d (%d used vs %d available)\n",c,SHALU,SHALUCount);}
          
                }
        
            printf("\n");
        
        /////////////////////////////////////////
        // test ALU resource untilisation conflicts
        
           int RRports=0;
           int WBports=0;
                       
                for(c=0;c<MAXSTAGES;c++)
                {
                    RRports=0;
                    WBports=0;
                                        
                     for(i=0;i<OpCount;i++)
                     {
                        if(IsStageRR(i,c)){ RRports++;}
                        if(IsStageWB(i,c)){ WBports++;}                        
                     }
                     
                     
                     if(RRports>ReadPorts) 
                     { printf(" READ PORTS Exceeded at c%02d (%d used vs %d available)\n",c,RRports,ReadPorts);}
                     
                     if(WBports>WritePorts) 
                     { printf(" WRITE PORTS Exceeded at c%02d (%d used vs %d available)\n", c,WBports,WritePorts);}
                     
                              
                }

        
        /////////////////////////////////////////
        // end of sequence of tests
        
           printf("\n-------------------------------------------\n\n");
        
        
}

void PipelineClass::DumpPipeline()
{
    int flag=0;
    int Max=0;
    
        
    printf("\n---- PIPELINE DIAGRAM----\n");
    
    
   for(int w=0;w<MAXSTAGES;w+=10)
   {
    for(int x=0;x<OpCount;x++)
    {
      printf("%03d: % 8s. % 8s. % 8s. % 8s  >> [I=%03d]  ",x
              ,Opcode[x],OperX[x]
              ,OperY[x],OperZ[x],x  );
              
        flag=0;
        
        for(int y = 0; y<10; y++) //y < MAXSTAGES; y++)
        {
            // detect blank cases (a)leading and (b)trailing
               if(flag==1)           { break; }
               if(Stage[x][y+w][0]==0) { printf("[  --  ]"); continue; }
            
            // print stage ID string
               printf("[%s]",Stage[x][y+w]);
            
            // find end of stage sequence
               if(Stage[x][y+1+w][0]==0){ flag=1; Max=y+w; }    
               
            
        }
        
        printf("\n");
    
    }
    
    printf("\n");
    printf("% 57s","");
    for(int y=0;y<10;y++){ printf("[C-%04d]",y+w);}
    
    printf("\n\n\n");
    
    if(flag==1){break;}
  }
}

void PipelineClass::InsertStage(int i, int c,char * s)
{
    for(int x=MAXSTAGES; x> c; x--)
    {
        strcpy(Stage[i][x],Stage[i][x-1]);
   
    }
   
    strcpy(Stage[i][c],s);
}

void PipelineClass::InsertStall(int i,int c)
{
         InsertStage(i,c,"------");
}

void PipelineClass::DeleteStage(int i, int c)
{
    for(int x = c; x<MAXSTAGES-1; x++)
    {
        strcpy(Stage[i][x],Stage[i][x+1]);  
    }

    strcpy(Stage[i][MAXSTAGES-1],"");
}


