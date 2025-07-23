# hft-simulator

A high-frequency trading (HFT) simulator written in C++.

## Project Structure

- `src/`      - Source code (core logic, engines, parsers)
- `include/`  - Header files (class declarations)
- `data/`     - Sample data files (CSV for orders, trades)
- `tests/`    - Unit and integration tests
- `scripts/`  - Utility scripts (build, run, etc.)

### Key Components
- **Order**: Represents a market order (ID, side, price, quantity, timestamp).
- **OrderBook**: Manages buy/sell orders, supports add, match, and cancel operations. Efficiently separates buy/sell sides and logs trades.
- **CSVParser**: Loads orders from CSV files for simulation.
- **StrategyEngine**: Simulates a basic market-making strategy, periodically submitting buy/sell orders at a configurable spread and interval.
- **TradeLogger**: Logs matched trades to CSV and prints summary statistics (total trades, volume, P&L).

## Build Instructions

### Prerequisites
- C++17 compatible compiler (e.g., g++, clang++)
- CMake (recommended)

### Build with CMake
```sh
mkdir build
cd build
cmake ..
cmake --build .
```

### Run Main Simulation
```sh
./hft-simulator
```

### Run Unit Tests
```sh
# From build/ directory
./test_order
./test_orderbook
```

## Data Files
- Place your input order CSV as `data/orders.csv` (columns: order_id, side, price, quantity, timestamp).
- Matched trades will be logged to `data/trades.csv`.

## Code Documentation

- All major classes and functions are commented in the codebase.
- See header files in `include/` for API documentation and usage.

---

Feel free to contribute or open issues! 