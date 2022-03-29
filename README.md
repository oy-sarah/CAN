# CAN
A project of fault localization tool.
This is a demo for nanoxml_v5, fault 3.
## Usage
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
We only use 4 projects from Defect4J because we only perform slicing technology successfully on this 4 projects. Concretely,  we use version 1, 5, 6, 7, 9, 11, 13, 14, 16, 17, 18, 22, 24 and 25 for chart, version 1, 12, 13, 16, 19, 20, 27, 35, 36, 39, 44, 45, 51, 54, 58, 61, 63 and 65 for lang, version 3, 40, 49, 50, 70, 78, 84, 90, 95, 96, 97 and 98 for math, version 2, 3, 4, 5, 7, 9 ,10, 16, 17, 19, 21 and 23 for time.

## Docker implementation
We also provide docker implementation
```shell
docker pull zz8477/ubuntu18.04:CAN
docker run -it --gpus all zz8477/ubuntu18.04:CAN /bin/bash
cd /home/CAN/fault_localization_CAN/
./installDL.sh
vim FL_result.txt
```

