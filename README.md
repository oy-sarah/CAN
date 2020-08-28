# CAN
A project of fault localization tool.
This is a demo for nanoxml_v5, fault 3.
# Usage
1. Requirements
python >= 3.6.2
dgl
gcc: 5.4.0

2. Construct graph of nanoxml_v5
we use dynamic backward slice, data and control dependence to construct a failure context, which is used to generate graph of nanoxml_v5. More technical details refer to constructing a failure context with dynamic backward slice, please check our previous paper XXX.
3. Run fault localization project
fault_localization :./runall_CAN.sh

4. Result
DLFinalresult.txt(fault localization result with rank　and Exam value)
