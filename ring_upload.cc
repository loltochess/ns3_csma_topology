/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*            Ring ! Network Topology  
// 
// 
//                          n2    10.1.2.0     n3
//                              \         /
//                               \       /
//                      10.1.1.0                  10.1.3.0
//                                 \   /
//                                  \ /
//                  n1        ㅡ ㅡ   n0 ㅡ ㅡ ㅡ ㅡ         n4
//                                  /  \
//                                 /    \
//                    10.1.6.0)                 (10.1.4.0)     
//                               /        \
//                              /          \
//                          n6    10.1.5.0     n5
//   
*/

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
#include "ns3/flow-monitor-module.h"





using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  float clientapptime = 2.0;
  float simTime = 11;  
  
//Change throughputtest variable from false to true during throughput test  
  bool throughputtest = true;


  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

  cmd.Parse (argc,argv);

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }




//////////////////////////////// Topology Creation ////////////////////////////////
//**Fill in the code  

// 1. Create NodeContainer


NodeContainer p2pNodes;
p2pNodes.Create(6);


// 2. Create P2P links and set attributes

//////// Your Code ////////
PointToPointHelper pointToPoint;
if (!throughputtest)
   {
        pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
   }
if (throughputtest)
   {
        pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("15kbps"));
   }
pointToPoint.SetChannelAttribute ("Delay", StringValue ("2s"));

// 3. Create NetDeviceContainer

//////// Your Code ////////
NetDeviceContainer devices1, devices2, devices3, devices4, devices5, devices6;
devices1.Add(pointToPoint.Install(p2pNodes.Get(0),p2pNodes.Get(1)));
devices2.Add(pointToPoint.Install(p2pNodes.Get(1),p2pNodes.Get(2)));
devices3.Add(pointToPoint.Install(p2pNodes.Get(2),p2pNodes.Get(3)));
devices4.Add(pointToPoint.Install(p2pNodes.Get(3),p2pNodes.Get(4)));
devices5.Add(pointToPoint.Install(p2pNodes.Get(4),p2pNodes.Get(5)));
devices6.Add(pointToPoint.Install(p2pNodes.Get(5),p2pNodes.Get(0)));


// 4. Create InternetStack and assign IP addresses

//////// Your Code ////////
InternetStackHelper stack;
stack.Install(p2pNodes);

Ipv4AddressHelper address;

Ipv4InterfaceContainer p2pInterfaces01, p2pInterfaces02, p2pInterfaces03, p2pInterfaces04, p2pInterfaces05, p2pInterfaces06;
address.SetBase("10.1.1.0","255.255.255.0");
p2pInterfaces01 = address.Assign(devices1);

address.SetBase("10.1.2.0","255.255.255.0");
p2pInterfaces02 = address.Assign(devices2);

address.SetBase("10.1.3.0","255.255.255.0");
p2pInterfaces03 = address.Assign(devices3);

address.SetBase("10.1.4.0","255.255.255.0");
p2pInterfaces04 = address.Assign(devices4);

address.SetBase("10.1.5.0","255.255.255.0");
p2pInterfaces05 = address.Assign(devices5);

address.SetBase("10.1.6.0","255.255.255.0");
p2pInterfaces06 = address.Assign(devices6);

// 5-1. Application for delay test 
 if (!throughputtest){

// Packet 1
  UdpEchoServerHelper echoServer1 (9);
//////// Your Code ////////
  ApplicationContainer serverApps1 = echoServer1.Install (p2pNodes.Get(0));
  serverApps1.Start (Seconds (1.0));
  serverApps1.Stop (Seconds (30.0));

//////// Your Code ////////
  UdpEchoClientHelper echoClient1 (p2pInterfaces01.GetAddress(0),9);
  echoClient1.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));

//////// Your Code ////////
  ApplicationContainer clientApps1 = echoClient1.Install (p2pNodes.Get(5));
  clientApps1.Start (Seconds (clientapptime));
  clientApps1.Stop (Seconds (30.0));
 }


// 5-2. Application for throughput test 
// delay test : c,s 6,1
// throughput test : c,s 4,1 2,5 3,6
 if (throughputtest){
 
// Packet 1
  UdpEchoServerHelper echoServer1 (9);
//////// Your Code ////////
  ApplicationContainer serverApps1 = echoServer1.Install (p2pNodes.Get (0));
  serverApps1.Start (Seconds (1.0));
  serverApps1.Stop (Seconds (simTime));

//////// Your Code ////////
  UdpEchoClientHelper echoClient1 (p2pInterfaces01.GetAddress(0),9);
  echoClient1.SetAttribute ("MaxPackets", UintegerValue (3));
  echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));

//////// Your Code ////////
  ApplicationContainer clientApps1 = echoClient1.Install (p2pNodes.Get (3));
  clientApps1.Start (Seconds (clientapptime));
  clientApps1.Stop (Seconds (simTime));


// Packet 2
  UdpEchoServerHelper echoServer2 (10);
//////// Your Code ////////
  ApplicationContainer serverApps2 = echoServer2.Install (p2pNodes.Get(4));
  serverApps2.Start (Seconds (1.0));
  serverApps2.Stop (Seconds (simTime));

//////// Your Code ////////
  UdpEchoClientHelper echoClient2 (p2pInterfaces05.GetAddress(0),10);
  echoClient2.SetAttribute ("MaxPackets", UintegerValue (3));
  echoClient2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));

//////// Your Code ////////
  ApplicationContainer clientApps2 = echoClient2.Install (p2pNodes.Get(1));
  clientApps2.Start (Seconds (clientapptime));
  clientApps2.Stop (Seconds (simTime));

// Packet 3
  UdpEchoServerHelper echoServer3 (11);
//////// Your Code ////////
  ApplicationContainer serverApps3 = echoServer3.Install (p2pNodes.Get(5));
  serverApps3.Start (Seconds (1.0));
  serverApps3.Stop (Seconds (simTime));

//////// Your Code ////////
  UdpEchoClientHelper echoClient3 (p2pInterfaces06.GetAddress(0),11);
  echoClient3.SetAttribute ("MaxPackets", UintegerValue (3));
  echoClient3.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient3.SetAttribute ("PacketSize", UintegerValue (1024));

//////// Your Code ////////
  ApplicationContainer clientApps3 = echoClient3.Install (p2pNodes.Get(2));
  clientApps3.Start (Seconds (clientapptime));
  clientApps3.Stop (Seconds (simTime));
 }
 
//////////////////////////////// End ////////////////////////////////

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  
  FlowMonitorHelper flowmonHelper;
  Ptr<FlowMonitor> monitor = flowmonHelper.InstallAll();

  Simulator::Stop(Seconds(100.0));
  Simulator::Run ();
  pointToPoint.EnablePcapAll ("second");


  // Flowmonitor for throughput and delay output

  monitor->CheckForLostPackets();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmonHelper.GetClassifier());
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();
  float rxDuration = (simTime - clientapptime);

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin(); iter != stats.end(); ++iter) {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (iter->first);

    std::cout << "Flow " << t.sourceAddress << " -> " << t.destinationAddress << "\n";
    std::cout << "  Tx Bytes: " << iter->second.txBytes << "\n";
    std::cout << "  Throughput: " << iter->second.rxBytes * 8.0 / rxDuration/ 1000.0 << "Kbps\n";
    std::cout << "  Mean delay:  " << 1000 * iter->second.delaySum.GetSeconds () / iter->second.rxPackets << " ms\n";
    
  }

 


  Simulator::Destroy ();
  return 0;
}
