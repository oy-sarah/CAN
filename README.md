# CAN
A project of fault localization tool.
This is a demo for nanoxml_v5, fault 3.
# Usage
1. Requirements:
requirements.txt

2. Source code and buggy line:
source code:StdXMLParser.java
buggy line:368 if (! XMLUtil.checkLiteral(this.reader, "CDATA")) 
correct format:if (! XMLUtil.checkLiteral(this.reader, "CDATA["))

3. Construct graph of nanoxml_v5:
we use dynamic backward slice, data and control dependence to construct a failure context, which is used to generate graph of nanoxml_v5. More technical details refer to constructing a failure context with dynamic backward slice, please check our previous paper XXX.

4. Run fault localization project:
fault_localization :./runall_CAN.sh

5. Result：
faultStatementRank.txt(fault localization result with rank and Exam value)

6. Datasets: 
chart, math, lang, and time are from Defects4J,http://defects4j.org. python, gzip and libtiff are collected from ManyBugs,http://repairbenchmarks.cs.umass.edu/ManyBugs/. space and the four separate releases of nanoxml are acquired from the SIR,http://sir.unl.edu/portal/index.php.
