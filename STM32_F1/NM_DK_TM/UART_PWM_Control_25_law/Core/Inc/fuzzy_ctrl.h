#ifndef FUZZY_CTRL_H_INCLUDED
#define FUZZY_CTRL_H_INCLUDED
void value_hlt_d(float*,float*, float);
void value_hlt_phi(float*,float*, float);
void find_beta(float*,float*,float*);
void rule_fuzzy(float*, float*,float*, float*);
float defuzzy_motorL(float*,float*);
float defuzzy_motorR(float*,float*);

#endif // FUZZY_CTRL_H_INCLUDED
