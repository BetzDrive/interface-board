# This script calculates the free values for the LM5066 following the equations
# in section 9.2.1.2 -> http://www.ti.com/lit/ds/symlink/lm5066.pdf
import numpy as np

# User Picked Values
I_LIM = 3.3333  # Amps
CL = 0  # Grounded
V_IN_MAX = 60  # Volts
V_DS = 100  # Volts (From the MOSFET)
C_OUT = 474.4e-6  # Ferads
V_UVH = 18.9
V_UVL = 16.1
V_OVH = 55.4
V_OVL = 52.5

# Recommended/System Values
V_CL = (26e-3, 50e-3)[not CL]  # Volts (Picked by the CL config)
V_SNS_MIN = 4e-3  # Volts (TI recommends no lower)
t_fault_multiplier = 2

# Hard Coded Values
v_timer = 3.9  # Volts
i_timer = 75e-6  # Amps

# Calculate Sense Resistor (Equation 5)
R_SNS = V_CL / I_LIM
print('R_SNS', R_SNS)

# Calculate Power Limit (Equations 10-12)
P_LIM_MIN = V_SNS_MIN * V_IN_MAX / R_SNS
print('P_LIM_MIN', P_LIM_MIN)
R_PWR = 1.4e5 * R_SNS * (P_LIM_MIN - (1.5e-3 * V_IN_MAX / R_SNS))
print('R_PWR: {:.3f} kOhms'.format(R_PWR/1000.0))

# Set Fault Timer (Equation 13-16) -> runs during startup
# We skip equation 13 as I_LIM * V_DS > P_LIM
t_start_max = C_OUT / 2.0 * (((V_IN_MAX**2.0) / P_LIM_MIN) + (P_LIM_MIN/I_LIM**2.0))
print('t_start_max: {:.3f} ms'.format(t_start_max * 1000))
t_start_suggested = t_start_max * t_fault_multiplier
C_TIMER = t_start_suggested * i_timer / v_timer
print('C_TIMER: {} uF'.format(C_TIMER*1e6))

# Check SOA
print('SOA calculation not implemented (we are well inside the region)')

# UVLO and OVLO Thresholds (Option B)
# UVLO
R1 = (V_UVH - V_UVL) / 20e-6
R2 = (2.48 * R1) / (V_UVL - 2.48)
# OVLO
R3 = (V_OVH - V_OVL) / 21e-6
R4 = (2.46 * R3) / (V_OVH - 2.46)
print('R1: {:.0f}, R2: {:.0f}, R3: {:.0f}, R4: {:.0f}'.format(R1, R2, R3, R4))
