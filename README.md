# Asheron's Fall

An Asheron's Call server implemented in LPC.  The client software can be
downloaded from the
[Turbine website](http://content.turbine.com/sites/clientdl/ac1/ac1install.exe).

To run the emulator, you'll need
[DGD](https://github.com/dworkin/dgd).

If you don't want to compile DGD yourself, you can also use Hydra, a binary
of which can be downloaded
[here](https://dworkin.dworkin.nl/hydra/).

You will also need the
[Cloud Server library](https://github.com/dworkin/cloud-server).

Either copy or create a softlink from `AsheronsFall/src` to 
`cloud-server/src/usr/Asheron`.  Next, create softlinks or copy the client
files `client_cell_1.dat` and `client_portal.dat` to
`AsheronsFall/src/dat/data`.

After starting the server, you can connect to it with

    acclient.exe -h 127.0.0.1 -p 9000 -a accountName --glsticketdirect password
