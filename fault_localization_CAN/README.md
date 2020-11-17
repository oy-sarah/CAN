# CAN
A project of fault localization tool.
This is a demo for nanoxml_v5, fault 3.
# Usage
1. Requirements
requirements.txt

2.source code and buggy line
source code:StdXMLParser.java
buggy line:368 if (! XMLUtil.checkLiteral(this.reader, "CDATA")) 
correct format:if (! XMLUtil.checkLiteral(this.reader, "CDATA["))

 1. Construct graph of nanoxml_v5
we use dynamic backward slice, data and control dependence to construct a failure context, which is used to generate graph of nanoxml_v5. More technical details refer to constructing a failure context with dynamic backward slice, please check our previous paper XXX.

 2. Run fault localization project
fault_localization :./runall_CAN.sh

 3. Result
faultStatementRank.txt(fault localization result with rank and Exam value)

 # Exam and time
 1. EXAM score of adding randomness to the failure contexts compared with CAN
![exam](https://raw.githubusercontent.com/oy-sarah/CAN-doc-source/master/images/exam.png)
 2. runtime
![runtime](https://raw.githubusercontent.com/oy-sarah/CAN-doc-source/master/images/table.png)
