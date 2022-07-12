
/* The blocks of code below was used to simulate the performance measures of TCP and UDP protocols under Wireless saturation scenario.
 * Network topology:
 *
 *   Ap    STA 
 *   *      *
 *   |      |
 *   n1     n2, n3, n4, n5, n6, ..., n35
 *
 * Multiple HT stations send packets to the access point.
 * The average throughput, packet delivery and loss ratio and end-to-end delay received was reported under 5 seconds for both
   protocols with increasing number of station nodes.
 */

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/flow-monitor-module.h"

NS_LOG_COMPONENT_DEFINE ("TCP and UDP performance measures in Wireless Condition based on increasing distance");

using namespace ns3;

int main (int argc, char *argv[]) {

        uint32_t payloadSize = 2448;                       /* Transport layer payload size in bytes */
        double distance = 10;                              /* Distance between the station nodes and AP node */
        std::string dataRate = "100Mbps";                  /* Application layer datarate. */
        std::string phyRate = "HtMcs7";                    /* Physical layer bitrate. */
        double simulationTime = 5;                         /* Simulation time in seconds. */
        bool pcapTracing = false;                          /* PCAP Tracing is enabled or not. */
        uint32_t nstation = 5;                             /* Number of Station nodes, beginning at 5 */

        /* Command line argument parser setup. */
        CommandLine cmd;
          cmd.AddValue ("payloadSize", "Payload size in bytes", payloadSize);
          cmd.AddValue ("dataRate", "Application data ate", dataRate);
          cmd.AddValue ("distance", "meters separation between nodes", distance);
          cmd.AddValue ("phyRate", "Physical layer bitrate", phyRate);
          cmd.AddValue ("simulationTime", "Simulation time in seconds", simulationTime);
          cmd.AddValue ("pcap", "Enable/disable PCAP Tracing", pcapTracing);
          cmd.AddValue ("nStation", "Number of wifi STA devices", nstation);
          cmd.Parse (argc, argv);

         /* No fragmentation and no RTS/CTS */
          Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("999999"));
          Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("999999"));


         /* Configure TCP Options */
         Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (payloadSize));


        WifiMacHelper wifiMac;
        WifiHelper wifiHelper;
        wifiHelper.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);

        /* Set up Legacy Channel */
        YansWifiChannelHelper wifiChannel;
        wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
        wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel", "Frequency", DoubleValue (5e9));

        /* Setup Physical Layer */
        YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
        wifiPhy.SetChannel (wifiChannel.Create ());
        wifiPhy.Set ("TxPowerStart", DoubleValue (10.0));
        wifiPhy.Set ("TxPowerEnd", DoubleValue (10.0));
        wifiPhy.Set ("TxPowerLevels", UintegerValue (1));
        wifiPhy.Set ("TxGain", DoubleValue (0));
        wifiPhy.Set ("RxGain", DoubleValue (0));
        wifiPhy.Set ("RxNoiseFigure", DoubleValue (10));
        wifiPhy.Set ("CcaMode1Threshold", DoubleValue (-79));
        wifiPhy.Set ("EnergyDetectionThreshold", DoubleValue (-79 + 3));
        wifiPhy.SetErrorRateModel ("ns3::YansErrorRateModel");
        wifiHelper.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                            "DataMode", StringValue (phyRate),
                                            "ControlMode", StringValue ("HtMcs0"));

        /* Create AP node */
        NodeContainer p2pNodes;
        p2pNodes.Create (1);
        NodeContainer apWifiNode = p2pNodes.Get (0);
        
        /* Create Staion nodes */
        NodeContainer staWifiNode;
        staWifiNode.Create (nstation);
         

        /* Configure AP */
        Ssid ssid = Ssid ("network");
        wifiMac.SetType ("ns3::ApWifiMac",
                        "Ssid", SsidValue (ssid));

        NetDeviceContainer apDevice;
        apDevice = wifiHelper.Install (wifiPhy, wifiMac, apWifiNode);

        /* Configure STA */
        wifiMac.SetType ("ns3::StaWifiMac",
                        "Ssid", SsidValue (ssid));

        NetDeviceContainer staDevices;
        staDevices = wifiHelper.Install (wifiPhy, wifiMac, staWifiNode);

        /* Mobility model */
        MobilityHelper mobility;
        Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
        positionAlloc->Add (Vector (0.0, 0.0, 0.0));
        positionAlloc->Add (Vector (distance, 0.0, 0.0));              

        mobility.SetPositionAllocator (positionAlloc);
        mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
        mobility.Install (apWifiNode);
        mobility.Install (staWifiNode);

        /* Internet stack */
        InternetStackHelper stack;
        stack.Install (networkNodes);

        Ipv4AddressHelper address;
        address.SetBase ("10.0.0.0", "255.255.255.0");
        Ipv4InterfaceContainer apInterface;
        apInterface = address.Assign (apDevice);
        Ipv4InterfaceContainer staInterface;
        staInterface = address.Assign (staDevices);

        /* Populate routing table */
        Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

        /* Install TCP/UDP Receiver on the access point */
        PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 9));
        ApplicationContainer sinkApp = sinkHelper.Install (apWifiNode);
        sink = StaticCast<PacketSink> (sinkApp.Get (0));

        /* Install TCP/UDP Transmitter on the station */
        OnOffHelper server ("ns3::TcpSocketFactory", (InetSocketAddress (apInterface.GetAddress (0), 9)));
        server.SetAttribute ("PacketSize", UintegerValue (payloadSize));
        server.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
        server.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
        server.SetAttribute ("DataRate", DataRateValue (DataRate (dataRate)));
        ApplicationContainer serverApp = server.Install (staWifiNode);


        FlowMonitorHelper flowmon;
        Ptr<FlowMonitor> monitor = flowmon.InstallAll();

                /* Start Applications */
                sinkApp.Start (Seconds (0.0));
                serverApp.Start (Seconds (1.0)); 

                /* Enable Traces */
                if (pcapTracing)
                  {
                    wifiPhy.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO);
                    wifiPhy.EnablePcap ("AccessPoint", apDevice);
                    wifiPhy.EnablePcap ("Station", staDevices);
                  }

                /* Start Simulation */
                Simulator::Stop (Seconds (simulationTime + 1));
                Simulator::Run ();
                Simulator::Destroy ();


         /* Reporting Simulation Parameters*/
        int j=0;
        float AvgThroughput = 0;
        uint32_t SentPackets = 0;
        uint32_t ReceivedPackets = 0;
        uint32_t LostPackets = 0;
        Time Jitter;
        Time Delay;

      Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
      std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();

          for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin (); iter != stats.end (); ++iter)
            {
            Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (iter->first);

                NS_LOG_UNCOND("----Flow ID:" <<iter->first);
                NS_LOG_UNCOND("Src Addr" <<t.sourceAddress << "Dst Addr "<< t.destinationAddress);
                NS_LOG_UNCOND("Sent Packets=" <<iter->second.txPackets);
                NS_LOG_UNCOND("Received Packets =" <<iter->second.rxPackets);
                NS_LOG_UNCOND("Lost Packets =" <<iter->second.txPackets-iter->second.rxPackets);
                NS_LOG_UNCOND("Packet delivery ratio =" <<iter->second.rxPackets*100/iter->second.txPackets << "%");
                NS_LOG_UNCOND("Packet loss ratio =" << (iter->second.txPackets-iter->second.rxPackets)*100/iter->second.txPackets << "%");
                NS_LOG_UNCOND("Delay =" <<iter->second.delaySum);
                NS_LOG_UNCOND("Jitter =" <<iter->second.jitterSum);
                NS_LOG_UNCOND("Throughput =" <<iter->second.rxBytes * 8.0/(iter->second.timeLastRxPacket.GetSeconds()-iter->second.timeFirstTxPacket.GetSeconds())/1024<<"Kbps");

                SentPackets = SentPackets +(iter->second.txPackets);
                ReceivedPackets = ReceivedPackets + (iter->second.rxPackets);
                LostPackets = LostPackets + (iter->second.txPackets-iter->second.rxPackets);
                AvgThroughput = AvgThroughput + (iter->second.rxBytes * 8.0/(iter->second.timeLastRxPacket.GetSeconds()-iter->second.timeFirstTxPacket.GetSeconds())/1024);
                Delay = Delay + (iter->second.delaySum);
                Jitter = Jitter + (iter->second.jitterSum);

                j = j + 1;

            }

        AvgThroughput = AvgThroughput/j;
        NS_LOG_UNCOND("--------Total Results of the simulation----------"<<std::endl);
        NS_LOG_UNCOND("Total sent packets  =" << SentPackets);
        NS_LOG_UNCOND("Total Received Packets =" << ReceivedPackets);
        NS_LOG_UNCOND("Total Lost Packets =" << LostPackets);
        NS_LOG_UNCOND("Packet Loss ratio =" << ((LostPackets*100)/SentPackets)<< "%");
        NS_LOG_UNCOND("Packet delivery ratio =" << ((ReceivedPackets*100)/SentPackets)<< "%");
        NS_LOG_UNCOND("Average Throughput =" << AvgThroughput<< "Kbps");
        NS_LOG_UNCOND("End to End Delay =" << Delay);
        NS_LOG_UNCOND("End to End Jitter delay =" << Jitter);
        NS_LOG_UNCOND("Total Flod id " << j);
        monitor->SerializeToXmlFile("wifi-tcp.xml", true, true);



          double averageThroughput = ((sink->GetTotalRx () * 8) / (1e6  * simulationTime));
          if (averageThroughput < 5)
            {
              NS_LOG_ERROR ("Obtained throughput is not in the expected boundaries!");
              exit (1);
            }
          std::cout << "\nAverage throughput: " << averageThroughput << " Mbit/s" << std::endl;
          return 0;

               
}
