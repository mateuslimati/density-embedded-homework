# Density Counter service

Counter service is a Linux service that maintains a single current count and modifies the count by accepting commands over TCP/IP socket.
## How this service could be integrated with systemd:

* First, copy counter-service.service to /etc/systemd/system folder:

```sh
sudo cp counter-service.service /etc/systemd/system
```

* The second step is does a manual reaload on systemd to load the new system service:

```sh
sudo systemctl daemon-reload
```

* After that you need to enable the daemon on boot with:

```sh
sudo systemctl enable counter-service
```

* Finally, you need to start the service:

```sh
sudo systemctl start counter-service
```

Counter service restart after any failure, and its errors alerts are redirect correctly to journal via syslog.