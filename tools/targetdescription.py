import yaml

class CPU:
    def __init__(self, arch, isa, abi):
        self.arch = arch
        self.isa = isa
        self.abi = abi

    def __repr__(self):
        return f"CPU(arch={self.arch}, isa={self.isa}, abi={self.abi})"

class Device:
    def __init__(self, address, type, size=None, driver=None):
        self.address = address
        self.type = type
        self.size = size
        self.driver = driver

    def __repr__(self):
        return f"Device(address={hex(self.address)}, type={self.type}, size={self.size}, driver={self.driver})"

class Board:
    def __init__(self, id, vendor, cpus, devices):
        self.id = id
        self.vendor = vendor
        self.cpus = {name: CPU(**details) for name, details in cpus.items()}
        self.devices = {name: Device(**details) for name, details in devices.items()}

    @classmethod
    def from_yaml(cls, yaml_content):
        data = yaml.safe_load(yaml_content)
        return cls(
            id=data['id'],
            vendor=data['vendor'],
            cpus=data['cpus'],
            devices=data['devices']
        )

    def get_memory_devices(self):
        return {name: device for name, device in self.devices.items() if device.type in ['memory', 'ram']}

    # XXX(pkoscik): this is not necessarily true, this is a property of the SoC - not the board
    def get_first_cpu(self):
        return next(iter(self.cpus.values()))

    def __repr__(self):
        return f"Board(id={self.id}, vendor={self.vendor}, cpus={self.cpus}, devices={self.devices})"
