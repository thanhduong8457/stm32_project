import numpy as np
import skfuzzy as fuzz
import matplotlib.pyplot as plt
from skfuzzy import control as ctrl
#from mpl_toolkits.mplot3d import Axes3D  # Required for 3D plotting

# New Antecedent/Consequent objects hold universe variables and membership
# functions
# INPUTS a = distance --- b = angle
a = 200
b = 20
#
distance = ctrl.Antecedent(np.arange(0, 400, 0.1), 'distance')
angle = ctrl.Antecedent(np.arange(-60, 60, 0.1), 'angle')

vr = ctrl.Consequent(np.arange(-1, 1, 0.01), 'vr')
vl = ctrl.Consequent(np.arange(-1, 1, 0.01), 'vl')
#
distance['VS'] = fuzz.trimf(distance.universe, [0, 0, 100])
distance['S'] = fuzz.trimf(distance.universe, [0, 100, 200])
distance['M'] = fuzz.trimf(distance.universe, [100, 200, 300])
distance['B'] = fuzz.trimf(distance.universe, [200, 300, 400])
distance['VB'] = fuzz.trimf(distance.universe, [300, 400, 400])
#
angle['NB'] = fuzz.trimf(angle.universe, [-60, -60, -30])
angle['NS'] = fuzz.trimf(angle.universe, [-60, -30, 0])
angle['ZE'] = fuzz.trimf(angle.universe, [-30, 0, 30])
angle['PS'] = fuzz.trimf(angle.universe, [0, 30, 60])
angle['PB'] = fuzz.trimf(angle.universe, [30, 60, 60])
#
vr['NB'] = fuzz.trimf(vr.universe, [-1, -1, -0.66])
vr['NM'] = fuzz.trimf(vr.universe, [-1, -0.66, -0.33])
vr['NS'] = fuzz.trimf(vr.universe, [-0.66, -0.33, 0])
vr['ZE'] = fuzz.trimf(vr.universe, [-0.33, 0, 0.33])
vr['PS'] = fuzz.trimf(vr.universe, [0, 0.33, 0.66])
vr['PM'] = fuzz.trimf(vr.universe, [0.33, 0.66, 1])
vr['PB'] = fuzz.trimf(vr.universe, [0.66, 1, 1])
#
vl['NB'] = fuzz.trimf(vr.universe, [-1, -1, -0.66])
vl['NM'] = fuzz.trimf(vr.universe, [-1, -0.66, -0.33])
vl['NS'] = fuzz.trimf(vr.universe, [-0.66, -0.33, 0])
vl['ZE'] = fuzz.trimf(vr.universe, [-0.33, 0, 0.33])
vl['PS'] = fuzz.trimf(vr.universe, [0, 0.33, 0.66])
vl['PM'] = fuzz.trimf(vr.universe, [0.33, 0.66, 1])
vl['PB'] = fuzz.trimf(vr.universe, [0.66, 1, 1])
#####
plt.ion()
distance.view()
plt.title('distance')
plt.ioff()

plt.ion()
angle.view()
plt.title('angle')
plt.ioff()

plt.ion()
vr.view()
plt.title('V_right')
plt.ioff()

plt.ion()
vl.view()
plt.title('V_left')
plt.ioff()
### Luật cho V_Right
rule1R = ctrl.Rule(distance['VS'] & angle['NB'], vr['NM'])
rule2R = ctrl.Rule(distance['VS'] & angle['NS'], vr['NM'])
rule3R = ctrl.Rule(distance['VS'] & angle['ZE'], vr['NM'])
rule4R = ctrl.Rule(distance['VS'] & angle['PS'], vr['NB'])
rule5R = ctrl.Rule(distance['VS'] & angle['PB'], vr['NB'])
rule6R = ctrl.Rule(distance['S'] & angle['NB'], vr['NS'])
rule7R = ctrl.Rule(distance['S'] & angle['NS'], vr['NS'])
rule8R = ctrl.Rule(distance['S'] & angle['ZE'], vr['NS'])
rule9R = ctrl.Rule(distance['S'] & angle['PS'], vr['NM'])
rule10R = ctrl.Rule(distance['S'] & angle['PB'], vr['NB'])
rule11R = ctrl.Rule(distance['M'] & angle['NB'], vr['PM'])
rule12R = ctrl.Rule(distance['M'] & angle['NS'], vr['PS'])
rule13R = ctrl.Rule(distance['M'] & angle['ZE'], vr['ZE'])
rule14R = ctrl.Rule(distance['M'] & angle['PS'], vr['NS'])
rule15R = ctrl.Rule(distance['M'] & angle['PB'], vr['NM'])
rule16R = ctrl.Rule(distance['B'] & angle['NB'], vr['PB'])
rule17R = ctrl.Rule(distance['B'] & angle['NS'], vr['PM'])
rule18R = ctrl.Rule(distance['B'] & angle['ZE'], vr['PS'])
rule19R = ctrl.Rule(distance['B'] & angle['PS'], vr['PS'])
rule20R = ctrl.Rule(distance['B'] & angle['PB'], vr['PS'])
rule21R = ctrl.Rule(distance['VB'] & angle['NB'], vr['PB'])
rule22R = ctrl.Rule(distance['VB'] & angle['NS'], vr['PB'])
rule23R = ctrl.Rule(distance['VB'] & angle['ZE'], vr['PM'])
rule24R = ctrl.Rule(distance['VB'] & angle['PS'], vr['PM'])
rule25R = ctrl.Rule(distance['VB'] & angle['PB'], vr['PM'])
### Luật cho V_Left
rule1L = ctrl.Rule(distance['VS'] & angle['NB'],  vl['NB'])
rule2L = ctrl.Rule(distance['VS'] & angle['NS'],  vl['NB'])
rule3L = ctrl.Rule(distance['VS'] & angle['ZE'],  vl['NM'])
rule4L = ctrl.Rule(distance['VS'] & angle['PS'],  vl['NM'])
rule5L = ctrl.Rule(distance['VS'] & angle['PB'],  vl['NM'])
rule6L = ctrl.Rule(distance['S'] & angle['NB'],  vl['NB'])
rule7L = ctrl.Rule(distance['S'] & angle['NS'],  vl['NM'])
rule8L = ctrl.Rule(distance['S'] & angle['ZE'],  vl['NS'])
rule9L = ctrl.Rule(distance['S'] & angle['PS'],  vl['NS'])
rule10L = ctrl.Rule(distance['S'] & angle['PB'], vl['NS'])
rule11L = ctrl.Rule(distance['M'] & angle['NB'],  vl['NM'])
rule12L = ctrl.Rule(distance['M'] & angle['NS'],  vl['NS'])
rule13L = ctrl.Rule(distance['M'] & angle['ZE'],  vl['ZE'])
rule14L = ctrl.Rule(distance['M'] & angle['PS'],  vl['PS'])
rule15L = ctrl.Rule(distance['M'] & angle['PB'],  vl['PM'])
rule16L = ctrl.Rule(distance['B'] & angle['NB'],  vl['PS'])
rule17L = ctrl.Rule(distance['B'] & angle['NS'],  vl['PS'])
rule18L = ctrl.Rule(distance['B'] & angle['ZE'],  vl['PS'])
rule19L = ctrl.Rule(distance['B'] & angle['PS'],  vl['PM'])
rule20L = ctrl.Rule(distance['B'] & angle['PB'],  vl['PB'])
rule21L = ctrl.Rule(distance['VB'] & angle['NB'],  vl['PM'])
rule22L = ctrl.Rule(distance['VB'] & angle['NS'],  vl['PM'])
rule23L = ctrl.Rule(distance['VB'] & angle['ZE'],  vl['PM'])
rule24L = ctrl.Rule(distance['VB'] & angle['PS'],  vl['PB'])
rule25L = ctrl.Rule(distance['VB'] & angle['PB'],  vl['PB'])
### Outputs V_Right
speedR_ctrl = ctrl.ControlSystem(rules = [rule1R, rule2R, rule3R, rule4R, rule5R, rule6R, rule7R, rule8R, rule9R, rule10R, rule11R, rule12R, rule13R, rule14R, rule15R, rule16R, rule17R, rule18R, rule19R, rule20R, rule21R, rule22R, rule23R, rule24R, rule25R])
speedR = ctrl.ControlSystemSimulation(speedR_ctrl)
speedR.input['distance'] = a
speedR.input['angle'] = b
speedR.compute()

plt.ion()
print(speedR.output['vr'])
vr.view(sim=speedR)
plt.title('VR')
plt.show()
plt.ioff()

###Outputs V_Left
speedL_ctrl = ctrl.ControlSystem(rules = [rule1L, rule2L, rule3L, rule4L, rule5L, rule6L, rule7L, rule8L, rule9L, rule10L, rule11L, rule12L, rule13L, rule14L, rule15L, rule16L, rule17L, rule18L, rule19L, rule20L, rule21L, rule22L, rule23L, rule24L, rule25L])
speedL = ctrl.ControlSystemSimulation(speedL_ctrl)
speedL.input['distance'] = a
speedL.input['angle'] = b
speedL.compute()

plt.ion()
print(speedL.output['vl'])
vl.view(sim=speedL)
plt.title('VL')
plt.show()
plt.ioff()
####
plt.show()








