Description
-----------
The GroupChat sample demonstrates the use of a subset of the Peer to peer APIs centered around Grouping. It shows a secure multiparty chat application into which you can authenticate people using a password or certificates.
When using the certificate model, the order of using the UI is:
Application instance #1:
* Create an identity
* Send identity to person creating group
Application instance #2:
* Create identity
* Create a group using that identity
* Create an invitation for identity received from application instance #1
* Send invitation to application instance #1

On Application instance #1, use this invitation to join the group. 


How to Build
------------
nmake

How to Run
----------
Type groupchat.exe from ...\PeerToPeer\GroupChat\LH_DEBUG\
Just use the UI!


Platforms supported
-------------------
Windows Vista (except Starter Edition)