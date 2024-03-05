import geni.portal as portal
# Import the ProtoGENI library.
import geni.rspec.pg as rspec
# Emulab specific extensions.
import geni.rspec.emulab as emulab

# Create a portal context, needed to defined parameters
pc = portal.Context()

# Create a Request object to start building the RSpec.
request = pc.makeRequestRSpec()

# Set up first host - receiver
node_receiver = request.XenVM("receiver")
node_receiver.addService(rspec.Execute(shell="bash", command="/usr/bin/sudo /usr/bin/apt update; /usr/bin/sudo /usr/bin/apt -y install pcmanfm python3 leafpad"))
node_receiver.startVNC()
node_receiver.routable_control_ip = True

# Give receiver some friends
node_sender = request.XenVM("sender")
node_sender.addService(rspec.Execute(shell="bash", command="/usr/bin/sudo /usr/bin/apt update; /usr/bin/sudo /usr/bin/apt -y install pcmanfm python3 leafpad"))
node_sender.startVNC()
node_sender.routable_control_ip = True

# Set up a network link
lan = request.LAN()
lan.best_effort = True

iface_receiver = node_receiver.addInterface("eth1")
iface_receiver.addAddress(rspec.IPv4Address("10.10.0.100", "255.255.255.0"))
lan.addInterface(iface_receiver)

iface_sender= node_sender.addInterface("eth1")
iface_sender.addAddress(rspec.IPv4Address("10.10.0.101", "255.255.255.0"))
lan.addInterface(iface_sender)

# Print the RSpec to the enclosing page.
pc.printRequestRSpec(request)
