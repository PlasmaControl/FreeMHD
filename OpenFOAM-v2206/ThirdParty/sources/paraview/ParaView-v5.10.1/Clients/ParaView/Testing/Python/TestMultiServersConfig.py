from paraview.simple import *
from paraview import servermanager, print_info, print_error

import time

# Make sure the test driver know that process has properly started
print_info ("Process started")
errors = 0

#-------------------- Helpers methods ----------------
def getHost(url):
   return url.split(':')[1][2:]

def getScheme(url):
   return url.split(':')[0]

def getPort(url):
   return int(url.split(':')[2])
#--------------------
import os

def findInSubdirectory(filename, subdirectory=''):
    if subdirectory:
        path = subdirectory
    else:
        path = os.getcwd()
    for root, dirs, names in os.walk(path):
        for name in names:
           if (name.find(filename) > -1) and ( (name.find('.dll') > -1) or (name.find('.so') > -1) or (name.find('.dylib') > -1)):
              return os.path.join(root, name)
    raise RuntimeError ('File not found')
#--------------------

print_info ("Start multi-server testing")

options = servermanager.vtkRemotingCoreConfiguration.GetInstance()
available_server_urls = options.GetServerURL().split('|')
built_in_connection = servermanager.ActiveConnection

# Test if the built-in connection is here
if (len(servermanager.Connections) != 1):
  errors += 1
  print_error("Error pvpython should be connected to a built-in session. Currently connected to ", servermanager.Connections)

url = available_server_urls[0]
print_info ("Connect to first server %s", url)
server1_connection = Connect(getHost(url), getPort(url))

# Test that we have one more connection
if (len(servermanager.Connections) != 2):
  errors += 1
  print_error("Error pvpython should be connected to a built-in session + one remote one. Currently connected to ", servermanager.Connections)

url = available_server_urls[1]
print_info ("Connect to second server %s", url)
server2_connection = Connect(getHost(url), getPort(url))

# Test that we have one more connection
if (len(servermanager.Connections) != 3):
  errors += 1
  print_error("Error pvpython should be connected to a built-in session + two remote one. Currently connected to ", servermanager.Connections)

print_info ("Available connections: %s", servermanager.Connections)

# Test that last created connection is the active one
if ( servermanager.ActiveConnection != server2_connection):
  errors += 1
  print_error("Error Invalid active connection. Expected %s and got %s", server2_connection, servermanager.ActiveConnection)

# Test that SetActiveConnection is working as expected
SetActiveConnection(server1_connection, globals())
if ( servermanager.ActiveConnection != server1_connection):
  errors += 1
  print_error("Error Invalid active connection. Expected %s and got %s", server1_connection, servermanager.ActiveConnection)

# Test that SetActiveConnection is working as expected
SetActiveConnection(built_in_connection, globals())
if ( servermanager.ActiveConnection != built_in_connection):
  errors += 1
  print_info ("Error Invalid active connection. Expected %s and got %s", built_in_connection, servermanager.ActiveConnection)

# Test that SetActiveConnection is working as expected
SetActiveConnection(server2_connection, globals())
if ( servermanager.ActiveConnection != server2_connection):
  errors += 1
  print_error("Error Invalid active connection. Expected %s and got %s", server2_connection, servermanager.ActiveConnection)

assert "PacMan" not in globals()

# Load plugin on server2
SetActiveConnection(server2_connection, globals())
LoadDistributedPlugin("PacMan", True, globals())

# Create PacMan on server2
assert "PacMan" in globals()
pacMan_s2 = PacMan()

# Switch to server1 and Create PacMan ==> This should fail
SetActiveConnection(server1_connection, globals())
try:
  pacMan_s1 = PacMan()
  errors += 1
  print_error("Error: PacMan should not be available on Server1")
except NameError:
  print_info ("OK: PacMan is not available on server1")

# Switch to server2 with globals and switch back to server1 with not updating the globals
SetActiveConnection(server2_connection, globals())

SetActiveConnection(server1_connection)

# Create PacMan ==> This should fail
try:
  pacMan_s1 = PacMan()
  errors += 1
  print_error("Error: PacMan should not be available on Server1")
except RuntimeError:
  print_info ("OK: PacMan is not available on server1")

# Make sure built-in as not the pacMan
SetActiveConnection(server2_connection, globals())
SetActiveConnection(built_in_connection, globals())
try:
  pacMan_builtin = PacMan()
  errors += 1
  print_error("Error: PacMan should not be available on built-in")
except NameError:
  print_info ("OK: PacMan is not available on built-in")

# Load plugin localy for built-in
# Create PacMan ==> This should be OK on built-in
SetActiveConnection(built_in_connection, globals())
LoadDistributedPlugin("PacMan", False, globals())
pacMan_builtin = PacMan()
print_info ("After loading the plugin locally in built-in, the PacMan definition is available")

# Switch to server1 and Create PacMan ==> This should fail
SetActiveConnection(server1_connection, globals())
try:
  pacMan_s1 = PacMan()
  errors += 1
  print_error("Error: PacMan should not be available on Server1")
except NameError:
  print_info ("OK: PacMan is still not available on server1")

# Disconnect and quit application...
Disconnect()
print_info ("Available connections after disconnect: %s", servermanager.Connections)
Disconnect()
print_info ("Available connections after disconnect: %s", servermanager.Connections)
Disconnect()
print_info ("Available connections after disconnect: %s", servermanager.Connections)

if errors > 0:
  raise RuntimeError ("An error occurred during the execution")
