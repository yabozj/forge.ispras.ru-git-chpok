#!/usr/bin/env python2

#import os
#import itertools
import ipaddr
import chpok_configuration
#import xml.etree.ElementTree as ET

def parse_bool(s):
    # this follows xsd:boolean
    if s in ("true", "1"):
        return True
    if s in ("false", "0"):
        return False

    raise ValueError(s)

def parse_bytes(s):
    multiplier = 1
    if s.endswith("K"):
        multiplier = 2 ** 10
        s = s[:-1]
    elif s.endswith("M"):
        multiplier = 2 ** 20
        s = s[:-1]

    return int(s) * multiplier

def parse_time(s):
    # note: CHPOK uses ms internally

    if s.endswith("ns"):
        ns = int(s[:-2])
        # see the end of the function
    elif s.endswith("ms"):
        return int(s[:-2])
    elif s.endswith("s"):
        return int(s[:-1]) * (10 ** 3)
    else:
        # assume nanoseconds
        ns = int(s) 
    
    if ns < (10 ** 6):
            raise ValueError("specified time less than 1ms (which won't work due to 1ms timer precision)")
    return ns // (10 ** 6)

class ArincConfigParser:

    def parse(self, root):
        """
        Returns chpok_configuration.Configuration object.
        """

        conf = chpok_configuration.Configuration()

        partname_to_index = {}

        for i, part in enumerate(root.find("Partitions").findall("Partition")):
            conf.partitions.append(self.parse_partition(part))

            partname_to_index[part.find("Definition").attrib["Name"]] = i

        conf.slots = self.parse_schedule(root.find("Schedule"), partname_to_index)

        conf.channels = self.parse_channels(root.find("Connection_Table"), conf)

        conf.network = self.parse_network(root.find("Network"))
        
        conf.validate()

        return conf
    
    def parse_partition(self, root):
        res = chpok_configuration.Partition()

        # FIXME identifier is currently ignored
        res.name = root.find("Definition").attrib["Name"]

        # FIXME support partition period, which is simply a fixed attribute
        #       with no real meaning (except it can be introspected)

        res.size = parse_bytes(root.find("Memory").attrib["Bytes"])
        res.num_threads = int(root.find("Threads").attrib["Count"])

        res.num_arinc653_buffers = int(root.find("ARINC653_Buffers").attrib["Count"])
        res.num_arinc653_blackboards = int(root.find("ARINC653_Blackboards").attrib["Count"])
        res.num_arinc653_events = int(root.find("ARINC653_Events").attrib["Count"])
        res.num_arinc653_semaphores = int(root.find("ARINC653_Semaphores").attrib["Count"])

        res.buffer_data_size = parse_bytes(root.find("ARINC653_Buffers").attrib["Data_Size"])
        res.blackboard_data_size = parse_bytes(root.find("ARINC653_Blackboards").attrib["Data_Size"])

        res.ports = self.parse_ports(root.find("ARINC653_Ports"))

        res.hm_table = self.parse_hm(root.find("HM_Table"))

        return res

    def parse_schedule(self, root, partname_to_index):
        slots = []

        for x in root.findall("Slot"):
            slot_type = x.attrib["Type"]

            if slot_type == "Spare":
                s = chpok_configuration.TimeSlotSpare()
            elif slot_type == "Partition":
                s = chpok_configuration.TimeSlotPartition()
                s.partition = partname_to_index[x.attrib["PartitionNameRef"]]
                s.periodic_processing_start = parse_bool(x.attrib["PeriodicProcessingStart"])
                s.name=x.attrib["PartitionNameRef"]
            elif slot_type == "Network":
                s = chpok_configuration.TimeSlotNetwork()
            elif slot_type == "Monitor":
                s = chpok_configuration.TimeSlotMonitor()
            elif slot_type == "GDB":
                s = chpok_configuration.TimeSlotGDB()
            else:
                raise ValueError("unknown slot type %r" % slot_type)

            s.duration = parse_time(x.attrib["Duration"])

            slots.append(s)

        return slots

    def parse_ports(self, root):
        res = []

        for qp in root.findall("Queueing_Port"):
            p = chpok_configuration.QueueingPort()

            p.name = qp.attrib["Name"]
            p.direction = qp.attrib["Direction"]
            p.max_nb_messages = int(qp.attrib["MaxNbMessage"])
            p.max_message_size = int(qp.attrib["MaxMessageSize"])

            res.append(p)

        for sp in root.findall("Sampling_Port"):
            p = chpok_configuration.SamplingPort()
            
            p.name = sp.attrib["Name"]
            p.direction = sp.attrib["Direction"]
            p.max_message_size = int(sp.attrib["MaxMessageSize"])
            p.refresh = parse_time(sp.attrib["Refresh"])

            res.append(p)

        return res

    def parse_channels(self, root, conf):
        res = []

        for ch in root.findall("Channel") if root else []:
            x = chpok_configuration.Channel()
            x.src = self.parse_connection(ch.find("Source")[0], conf)
            x.dst = self.parse_connection(ch.find("Destination")[0], conf)

            res.append(x)

        return res

    def parse_connection(self, root, conf):
        if root.tag == "Standard_Partition":
            res = chpok_configuration.LocalConnection()
            res.port = conf.get_port_by_partition_and_name(
                root.attrib["PartitionName"],
                root.attrib["PortName"]
            )
        elif root.tag == "UDP":
            res = chpok_configuration.UDPConnection()

            #print('ipaddr: ', root.attrib['IP'])
            res.host = ipaddr.IPAddress(root.attrib["IP"])
            res.port = int(root.attrib["Port"])
        else:
            raise RuntimeError("unknown connection tag name %r" % root.tag)

        return res

    def parse_network(self, root):
        if root is None:
            return None

        res = chpok_configuration.NetworkConfiguration()

        res.ip = ipaddr.IPAddress(root.attrib["IP"])
        
        #if "MAC" in root.attrib:
        #    res.mac = bytes(int(x, 16) for x in root.attrib["MAC"].split(":"))
        #else:
        #    res.mac = None

        return res

    def parse_hm(self, root):
        if root is None:
            return None

        res = []

        for x in root.findall("Error"):
            res.append((
                x.attrib["Code"], # kind
                "POK_ERROR_LEVEL_" + x.attrib["Level"], # level
                "POK_ERROR_ACTION_" + x.attrib["Action"], # action
                "POK_ERROR_KIND_" + x.attrib["ErrorCode"], # target code
            ))

        return res
'''
def parse_args():
    import argparse

    parser = argparse.ArgumentParser()

    parser.add_argument("directory", type=str)
    parser.add_argument("--xml", required=True, type=argparse.FileType("r"))

    return parser.parse_args()

def main():
    args = parse_args()

    # XXX directory names are hardcoded for now
    partition_dirs = (os.path.join(args.directory, "pr%d" % i) for i in itertools.count(1))
    partition_dirs = list(itertools.islice(partition_dirs, 255)) # XXX limited to 255 partitions right now
    kernel_dir = os.path.join(args.directory, "kernel")

    root = ET.parse(args.xml)

    parser = ArincConfigParser()
    conf = parser.parse(root)
    
    chpok_configuration.write_configuration(conf, kernel_dir, partition_dirs)

if __name__ == "__main__":
    main()
'''