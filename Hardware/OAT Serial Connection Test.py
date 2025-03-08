import asyncio
import serial
import serial.tools.list_ports

class OATMount:
    def __init__(self):
        self.comm_handler = None

    @staticmethod
    def list_available_ports():
        ports = serial.tools.list_ports.comports()
        return [(port.device, port.description) for port in ports]

    async def connect_to_oat(self, device):
        try:
            self.comm_handler = serial.Serial(
                port=device,
                baudrate=2400,
                timeout=2,
                dsrdtr=False,
                rtscts=False
            )
            
            self.comm_handler.dtr = False
            self.comm_handler.rts = False
            
            self.comm_handler.port = device
            self.comm_handler.open()
            
            self.comm_handler.dtr = False
            self.comm_handler.rts = False
            
            return True, ""
            
        except Exception as e:
            if self.comm_handler:
                self.comm_handler.close()
            return False, str(e)

    async def send_command(self, command):
        try:
            full_command = f":{command}#"
            self.comm_handler.write(full_command.encode())
            response = self.comm_handler.readline().decode().strip()
            return response
        except Exception as e:
            return f"Error: {str(e)}" 

async def main():
    mount = OATMount()
    
    print("\nAvailable Serial Ports:")
    for port, desc in mount.list_available_ports():
        print(f"  {port}: {desc}")
    
    success, message = await mount.connect_to_oat("COM5")
    
    if success:
        print("\nConnection successful!")
        commands = ["GVP", "GVN", "XGM"]
        
        for cmd in commands:
            response = await mount.send_command(cmd)
            print(f"Send Command: {cmd} - Response: {response}")
    else:
        print(f"\nConnection failed: {message}")

if __name__ == "__main__":
    asyncio.run(main())