Electribe reverse engineering
=============================

<center><img src="https://github.com/coderofsalvation/electribe-emx-esx-reverse-engineering/blob/master/doc/tribes.jpg" style="height:200px"></center>

A gitrepo which represents people's attempts to document our research of 
the inner workings of the electribe emx/esx.
All the firmware files are here for anyone to play with.

# Everything is in the Wiki

All gathered information is stored in the [Wiki](wiki), the repo itself serves a storage of data- and researchfiles mentioned in the wiki.

# Questions/Roadmap:

Following questions are controversial and will eventually break somebody's electribe:

Q: Is it worth it at all? :D

Q: can the memory be upgraded, and would the OS still work.

Q: can a bricked electribe be overwritten/restored with the original flash?

A: maybe not! see [this issue](https://github.com/coderofsalvation/electribe-emx-esx-reverse-engineering/issues/1)

Q: can somebody learn something from the JTAG connector on the PCB?

Q: what does the EMX/ESX setting on the PCB board do (indicates control surface?)

Q: what modifications are needed in order to succesfully flash an ESX with EMX firmware (and vice versa)

Q: what is the ROM ICE chip on page 1 of the service manual

# Tools used so far: 

* binwalk
* hexdump
* hexdiff 
* vim

# Scope

* The scope is to enable anyone to improve the firmware using free tools
* Tools as free tools: linux-tools, or windows tools which run in wine as well
