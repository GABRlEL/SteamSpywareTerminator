<div align="center">
  <h1>SteamSpywareTerminator</h1> (fuck you, burtonJ)
</div>

Work in progress (currently only logs thread initialization) library that once injected into steam, aims at blocking CSteamEngine::CFillMachineInfoThread which is a hidden thread that collects encrypted information regarding your hard drives, network adapters and optical peripherals with the lame purpose of blocking all your alternate accounts and make you lose thousands of money (and I don't really want to pay a dime more for burtonJ's salary)

## Reverse Engineering

You can find the instruction by looking up this string in IDA and xref:

<img src="https://i.imgur.com/kWk9Vi6.png">

You can't just terminate or return the thread, but interesting possibilities open up. That function is part of the only function (besides constructor, initiator and terminator/deconstructor) of a virtual table that inherits CScheduledFunction<CSteamEngine>.

'MachineGuid' is collected from registry and SHA1'd, possibly with a "BB3" constant? I know fuck nothing about encryption

<img src="https://i.imgur.com/G2W6vc1.png">

Other device registry information is then collected

<img src="https://i.imgur.com/diWvpFz.png">

At the same time, CIMV2 WMIC queries to retrieve network adapters are ran

<img src="https://i.imgur.com/WvnjhbH.png">

This time the above data is encrypted with the constant "FF2"? An interesting approach without blocking the entire thread could be changing these constants to alter the results of the digest. If Steam would fall back to not encrypting these strings they could be against GDPR as they are collecting information that can be classified as "personal data"

More queries follow up, the amount of data they collect is creepy.

"SELECT * FROM Win32_DiskPartition", they check BootPartitions, DeviceID, DiskIndex, again "SELECT * FROM Win32_DiskDrive", more stuff, then once AGAIN "SELECT * FROM Win32_PhysicalMedia", they check serials and manufacturer.

Encrypted again, "3B3" constant (?)

Finally, the second argument of the callee is encrypted (if not NULL) with the "333" constant (?)
