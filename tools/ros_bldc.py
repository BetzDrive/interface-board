#!/usr/bin/env python

from bldc_controller import BrushlessControllerChain
import time
import math
import signal
import sys
import rospy
from sensor_msgs.msg import JointState
from std_msgs.msg import Float64
from std_msgs.msg import Float32
from comms import *




ENCODER_ANGLE_PERIOD = 1 << 14
MAX_CURRENT = 2.8
CONTROL_LOOP_FREQ = 500
mapping = {2: "sroll_joint", 3: "elbow_joint", 1: "eroll_joint", 5: "lift_joint"} # mapping of id to joints

#################################################################################################
############### Helper Functions ################################################################
#################################################################################################

def setpointFunction(t):
	return math.sin(t * 2 * math.pi * 0.5) * 5.0

def getEncoderAngleRadians(device, key):
	return float(device.getEncoder(key)) / ENCODER_ANGLE_PERIOD * 2 * math.pi

def makeSetCommand(key):
	def setCommand(key, msg):
	 	global device
		effort_raw = msg.data
		effort_filtered = effort_raw
	     
		if effort_filtered > MAX_CURRENT:
	    	effort_filtered = MAX_CURRENT
		elif effort_filtered < -MAX_CURRENT:
			effort_filtered = -MAX_CURRENT

		if device.name == 'lift_joint':
			effort_filtered = -effort_filtered

		device.setCurrent(key, effort_filtered)
		print "I heard " + str(effort_filtered)
	return lambda msg: setCommand(key, msg)

##################################################################################################

def main():
	rospy.init_node('jointInterface', anonymous=True)
	rate = rospy.Rate(CONTROL_LOOP_FREQ)
	global device
	# Find and connect to the stepper motor controller
	port = sys.argv[1]
	s = serial.Serial(port=port, baudrate=3000000)
	print s.BAUDRATES
	device = BLDCControllerClient(s)

	pubArray = {}
	pubCurrArray = {}
	subArray = {}
	for key in mapping:
		pubArray[key] = rospy.Publisher("/DOF/" + mapping[key] + "_State", JointState, queue_size=10) 
		pubCurrArray[key] = rospy.Publisher("/DOF/" + mapping[key] + "_Current", Float32, queue_size=10)
		subArray[key] = rospy.Subscriber("/DOF/" + mapping[key] + "_Cmd", Float64, makeSetCommand(key))

	# Set up a signal handler so Ctrl-C causes a clean exit
	def sigintHandler(signal, frame):
		# device.setParameter('iq_s', 0)   ###############################################################--> must fix at some point
		# device.setControlEnabled(False) ################################################################--> must fix at some point
		print 'quitting'
		sys.exit()
	signal.signal(signal.SIGINT, sigintHandler)

	# Set current to zero, enable current control loop
	for key in mapping:
		# device.setParameter(key, 'id_s', 0) 
		# device.setParameter(key, 'iq_s', 0) 
		device.setControlEnabled(key, 0)########################### change to 1 when you want to actually control

	#angle = 0.0   # Treat the starting position as zero
	#last_mod_angle = getEncoderAngleRadians(device)
	start_time = time.time()
	time_previous = start_time
	angle_previous_mod = {}
	angle_accumulated = {}
	for key in mapping:
		angle_previous_mod[key] = getEncoderAngleRadians(device, key)
		angle_accumulated[key] = 0.0
  
	
  
	while not rospy.is_shutdown():
		for key in mapping:
			# Compute the desired setpoint for the current time
			jointName = mapping[key]

			loop_start_time = time.time()
			mod_angle = getEncoderAngleRadians(device, key)
			delta_time = loop_start_time - time_previous
			time_previous = loop_start_time
			delta_angle = (mod_angle - angle_previous_mod[key] + math.pi) % (2 * math.pi) - math.pi
			angle_previous_mod[key] = mod_angle
			angle_accumulated[key] = angle_accumulated[key] + delta_angle


			jointMsg = JointState()
			jointMsg.name = [jointName]
			jointMsg.position = [getEncoderAngleRadians(device, key)]
			#jointMsg.velocity = [device.getVelocity(key)]
			jointMsg.effort = [0.0] 
			pub[key].publish(jointMsg)
			print("name: " + jointName + "  position: " + str(jointMsg.position))
			rospy.spin
			time.sleep(max(0.0, loop_start_time + 1.0 / CONTROL_LOOP_FREQ - time.time()))

			currMsg = Float32()
			currMsg.data = float(0)
			pubCurr[key].publish(currMsg)
    



if __name__ == '__main__':
  main()
