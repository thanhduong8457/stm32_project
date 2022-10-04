#include "fuzzy_ctrl.h"
//
float data_hlt_d[5] = {0, 60, 120, 180, 240};
float data_hlt_phi[5] = {-60, -30, 0, 30, 60};

//
float data_motorL[7] = {-1, -0.666, -0.333, 0, 0.333, 0.666, 1};
float data_motorR[7] = {-1, -0.666, -0.333, 0, 0.333, 0.666, 1};

//
float rule_motorL[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float rule_motorR[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//
float muy_d[5] = {0,0,0,0,0};
float muy_phi[5] = {0,0,0,0,0};

float beta[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//

void value_hlt_d(float* data_d, float* muy_d, float d)
{
    if (d==data_d[0])
		{
        muy_d[0] = 1;
        muy_d[1] = 0;
        muy_d[2] = 0;
        muy_d[3] = 0;
        muy_d[4] = 0;
    }
    else if (d < data_d[1])
		{
        muy_d[0] = (data_d[1] - d) / (data_d[1] - data_d[0]);
        muy_d[1] = (d - data_d[0]) / (data_d[1] - data_d[0]);
        muy_d[2] = 0;
        muy_d[3] = 0;
        muy_d[4] = 0;
    }
    else if(d < data_d[2])
		{
        muy_d[0] = 0;
        muy_d[1] = (data_d[2] - d) / (data_d[2] - data_d[1]);
        muy_d[2] = (d - data_d[1]) / (data_d[2] - data_d[1]);
        muy_d[3] = 0;
        muy_d[4] = 0;
    }
    else if(d < data_d[3])
		{
        muy_d[0] = 0;
        muy_d[1] = 0;
        muy_d[2] = (data_d[3] - d) / (data_d[3] - data_d[2]);
        muy_d[3] = (d - data_d[2]) / (data_d[3] - data_d[2]);
        muy_d[4] = 0;
    }
    else if(d < data_d[4])
		{
        muy_d[0] = 0;
        muy_d[1] = 0;
        muy_d[2] = 0;
        muy_d[3] = (data_d[4] - d) / (data_d[4] - data_d[3]);
        muy_d[4] = (d - data_d[3]) / (data_d[4] - data_d[3]);
    }
    else 
		{
        muy_d[0] = 0;
        muy_d[1] = 0;
        muy_d[2] = 0;
        muy_d[3] = 0;
        muy_d[4] = 1;
    }
}

void value_hlt_phi(float* data_phi, float* muy_phi, float phi)
{
    if (phi==data_phi[0])
		{
        muy_phi[0] = 1;
        muy_phi[1] = 0;
        muy_phi[2] = 0;
        muy_phi[3] = 0;
        muy_phi[4] = 0;
    }
    else if (phi < data_phi[1])
		{
        muy_phi[0] = (data_phi[1] - phi) / (data_phi[1] - data_phi[0]);
        muy_phi[1] = (phi - data_phi[0]) / (data_phi[1] - data_phi[0]);
        muy_phi[2] = 0;
        muy_phi[3] = 0;
        muy_phi[4] = 0;
    }
    else if(phi < data_phi[2])
		{
        muy_phi[0] = 0;
        muy_phi[1] = (data_phi[2] - phi) / (data_phi[2] - data_phi[1]);
        muy_phi[2] = (phi - data_phi[1]) / (data_phi[2] - data_phi[1]);
        muy_phi[3] = 0;
        muy_phi[4] = 0;
    }
    else if(phi < data_phi[3])
		{
        muy_phi[0] = 0;
        muy_phi[1] = 0;
        muy_phi[2] = (data_phi[3] - phi) / (data_phi[3] - data_phi[2]);
        muy_phi[3] = (phi - data_phi[2]) / (data_phi[3] - data_phi[2]);
        muy_phi[4] = 0;
    }
    else if(phi < data_phi[4])
		{
        muy_phi[0] = 0;
        muy_phi[1] = 0;
        muy_phi[2] = 0;
        muy_phi[3] = (data_phi[4]-phi) / (data_phi[4] - data_phi[3]);
        muy_phi[4] = (phi-data_phi[3]) / (data_phi[4] - data_phi[3]);

    }
    else
		{
        muy_phi[0] = 0;
        muy_phi[1] = 0;
        muy_phi[2] = 0;
        muy_phi[3] = 0;
        muy_phi[4] = 1;
    }
}

void find_beta(float* muy_d, float* muy_phi, float* beta)
{
   int a=0;
   for(int i=0;i<5;i++)
	 {
    for(int j=0;j<5;j++)
		{
       beta[a] = muy_phi[i]*muy_d[j];
       a++;
    }
   }
}

void rule_fuzzy(float* rule_motorL, float* rule_motorR, float* data_motorL, float* data_motorR)
{																			//phi		distance		uk
    rule_motorL[0] = data_motorL[0];//				VS					NB
    rule_motorL[1] = data_motorL[0];//				S						NB
    rule_motorL[2] = data_motorL[1];//	NB		M						NM
    rule_motorL[3] = data_motorL[4];//				B						PS
    rule_motorL[4] = data_motorL[5];//				VB					PM

    rule_motorL[5] = data_motorL[0];
    rule_motorL[6] = data_motorL[1];
    rule_motorL[7] = data_motorL[2];
    rule_motorL[8] = data_motorL[4];
    rule_motorL[9] = data_motorL[5];

    rule_motorL[10] = data_motorL[1];
    rule_motorL[11] = data_motorL[2];
    rule_motorL[12] = data_motorL[3];
    rule_motorL[13] = data_motorL[4];
    rule_motorL[14] = data_motorL[5];

    rule_motorL[15] = data_motorL[1];
    rule_motorL[16] = data_motorL[2];
    rule_motorL[17] = data_motorL[4];
    rule_motorL[18] = data_motorL[5];
    rule_motorL[19] = data_motorL[6];

    rule_motorL[20] = data_motorL[1];
    rule_motorL[21] = data_motorL[2];
    rule_motorL[22] = data_motorL[5];
    rule_motorL[23] = data_motorL[6];
    rule_motorL[24] = data_motorL[6];

    //////////////////////

    rule_motorR[0] = data_motorR[1];
    rule_motorR[1] = data_motorR[2];
    rule_motorR[2] = data_motorR[5];
    rule_motorR[3] = data_motorR[6];
    rule_motorR[4] = data_motorR[6];

    rule_motorR[5] = data_motorR[1];
    rule_motorR[6] = data_motorR[2];
    rule_motorR[7] = data_motorR[4];
    rule_motorR[8] = data_motorR[5];
    rule_motorR[9] = data_motorR[6];

    rule_motorR[10] = data_motorR[1];
    rule_motorR[11] = data_motorR[2];
    rule_motorR[12] = data_motorR[3];
    rule_motorR[13] = data_motorR[4];
    rule_motorR[14] = data_motorR[5];

    rule_motorR[15] = data_motorR[0];
    rule_motorR[16] = data_motorR[1];
    rule_motorR[17] = data_motorR[2];
    rule_motorR[18] = data_motorR[4];
    rule_motorR[19] = data_motorR[5];

    rule_motorR[20] = data_motorR[0];
    rule_motorR[21] = data_motorR[0];
    rule_motorR[22] = data_motorR[1];
    rule_motorR[23] = data_motorR[4];
    rule_motorR[24] = data_motorR[5];
}

float defuzzy_motorL(float* beta,float* rule_motorL)
{
    float temp1 = 0, temp2 = 0;
    for(int i=0;i<25;i++)
		{
			temp1 = temp1 + beta[i];
			temp2 = temp2 + beta[i]*rule_motorL[i];
		}
    return (temp2/temp1);
}

float defuzzy_motorR(float* beta,float* rule_motorR)
{
    float temp1=0, temp2=0;
    for(int i=0;i<25;i++)
		{
		temp1 = temp1 + beta[i];
   	temp2 = temp2 + beta[i]*rule_motorR[i];   
		}
    return (temp2/temp1);
}

