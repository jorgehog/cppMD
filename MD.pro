TEMPLATE = subdirs
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += ordered

SUBDIRS += ignis app

BUILD_DIR = $$OUT_PWD

#   0
#   1.1848
#   1.7772
#   0.5924
#   0.0886
#   9.2288
#   7.5568
#   8.1017
#   0.5209
#   6.6153

#Thermostat@MainMesh             0
#Density@heatUpper               1.18478
#Density@coolMiddle              1.77717
#Density@heatLower               0.59239
#Pressure@coolMiddle             0.0886324
#Pressure@pressureTop            9.22882
#Pressure@pressureBottom         7.55681
#Thermostat@heatUpper            8.10174
#Thermostat@coolMiddle           0.520862
#Thermostat@heatLower            6.61531
