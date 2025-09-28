# AirDB ✈️
A high-performance aerospace parts comparison system that scrapes and aggregates pricing and availability data from multiple sources in real-time.

## 🚀 Features

- **Multi-source Scraping**: Simultaneously queries multiple aerospace parts suppliers
- **Real-time Data**: Live pricing and availability information
- **XML-based Parsing**: Robust HTML/XML parsing with CSS selector support
- **High Performance**: Built on C++/Nodepp for exceptional scraping speed
- **Unified Interface**: Consistent data format across all suppliers

## 📦 Supported Data Sources

| Supplier | Status | Parts Coverage |
|----------|---------|----------------|
| SkyParts Inc. | ✅ Live | Rotables, Avionics |
| AeroSupply Direct | ✅ Live | Consumables, Hardware |
| GlobalAero Parts | 🚧 Testing | Engines, Components |
| AviationStock | ✅ Live | Fasteners, Instruments |

## 🛠️ Tech Stack

- **Nodepp** - C++ framework for high-performance networking and scraping
- **XML Parser** - CSS-selector based DOM manipulation

## 📋 Prerequisites

- C++11 or newer
- Nodepp framework
- OpenSSL development libraries

## 🔧 Installation

```bash
# Clone the repository
git clone https://github.com/EDBC_REPO/airdb.git ; cd airdb

# Install dependencies (assuming Nodepp is available)
git submodule update --init --recursive

# Build with Bash
./compile.sh
```