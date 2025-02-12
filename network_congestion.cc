#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

void GenerateTraffic(std::string scenario) {
    NodeContainer nodes;
    nodes.Create(5);  // 5-node network

    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
    p2p.SetChannelAttribute("Delay", StringValue("5ms"));

    NetDeviceContainer devices = p2p.Install(nodes);
    InternetStackHelper stack;
    stack.Install(nodes);

    Ipv4AddressHelper address;
    address.SetBase("192.168.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = address.Assign(devices);

    uint16_t port = 9;
    ApplicationContainer serverApp;

    PacketSinkHelper sink("ns3::TcpSocketFactory",
                          InetSocketAddress(Ipv4Address::GetAny(), port));
    serverApp = sink.Install(nodes.Get(4));  // Last node as server

    serverApp.Start(Seconds(1.0));
    serverApp.Stop(Seconds(10.0));

    ApplicationContainer clientApp;
    OnOffHelper client("ns3::TcpSocketFactory",
                       InetSocketAddress(interfaces.GetAddress(4), port));
    client.SetAttribute("DataRate", StringValue(scenario == "congested" ? "50Mbps" : "5Mbps"));
    client.SetAttribute("PacketSize", UintegerValue(1024));

    clientApp = client.Install(nodes.Get(0));
    clientApp.Start(Seconds(2.0));
    clientApp.Stop(Seconds(10.0));

    FlowMonitorHelper flowMonitor;
    Ptr<FlowMonitor> monitor = flowMonitor.InstallAll();

    Simulator::Stop(Seconds(10.0));
    Simulator::Run();

    monitor->SerializeToXmlFile(scenario + "-flow.xml", true, true);

    Simulator::Destroy();
}

int main() {
    std::cout << "Running Normal Traffic Scenario..." << std::endl;
    GenerateTraffic("normal");

    std::cout << "Running Congested Traffic Scenario..." << std::endl;
    GenerateTraffic("congested");

    std::cout << "Simulation Completed!" << std::endl;
    return 0;
}
