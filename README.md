# StepmanSocketLights
Lights data on UDP socket for StepMania/OpenITG/NotITG

Used Visual Studio 2017, should compile as-is.

Just drop the resulting `parallel_lights_io.dll` in the `Program` folder of StepMania.
Remember to set `LightsDriver=Parallel` in `StepMania.ini`.

You also need a `socket_lights.txt` in the `Program` folder.
Default values are:
```
127.0.0.1
27015
```

Packet contains three bytes followed by a null byte.
The bits set on those three bytes are exactly the same as on the old parallel lights driver.