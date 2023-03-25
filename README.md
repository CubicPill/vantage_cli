# Vantage CLI

Vantage CLI is a command line interface to modify some settings provided by Lenovo Vantage.

(Windows only since it needs the DLL provided by Lenovo Vantage)

Currently, it only supports changing the charging mode.

If you are just a regular user looking for replacement of Lenovo Vantage, please check out 
[IdeapadToolkit](https://github.com/reagcz/IdeapadToolkit) which comes with GUI.

## Installation
### Prerequisites
- Lenovo Vantage supports all settings provided by this utility
- PowerBattery.dll from Lenovo Vantage (Included in the repository, Lenovo Vantage Version: 10.2301.8.0)
- Lenovo Intelligent Thermal Solution Driver

### Download
Download the latest release from [here]()

### Build from source
```bash
$ git clone https://github.com/CubicPill/vantage_cli.git
$ cd vantage_cli
$ cmake .
```


## Usage

```bash
$ vantage_cli.exe
Error: No argument provided
Usage: vantage_cli.exe <operation> [mode]
Supported operations: get_charging_status, set_charging_status
Supported modes: c(conservative), n(normal), r(rapid)
```


## Special Thanks to 

[IdeapadToolkit](https://github.com/reagcz/IdeapadToolkit). This project is basically a subset of `IdeapadToolkit`, using CLI instead of GUI for some automation purpose.

## Other useful utilities if you are using Lenovo laptops
[Coxxs/LogoDiy](https://github.com/Coxxs/LogoDiy)

[BartoszCichecki/LenovoLegionToolkit](https://github.com/BartoszCichecki/LenovoLegionToolkit)

[ViRb3/LenovoController](https://github.com/ViRb3/LenovoController)