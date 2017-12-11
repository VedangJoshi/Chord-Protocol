# Chord-Protocol
### Code to simulate the Chord protocol
<br>
#### Features<br>
##### (1) Node joins at the correct location in the Chord network
The node joins the network using a given node, which helps it to attach itself at the correct location.

##### (2) Correct finger tables printed for every node
The code prints finger table entries for its sucessors at least at a location 2 ^ (i-1), where 0 <= i <= m, on the Chord ring.

##### (3) Correct keys moved from successor to the joined node
The successor's suitable keys, if any present, are transferred to the newly attached node.

##### (4) Key searches based on finger table 
A key can be searched on a random node, even if the key isn't present on that node.
