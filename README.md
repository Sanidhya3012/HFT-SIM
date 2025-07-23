# hft-simulator

A high-frequency trading (HFT) simulator written in C++17 that provides advanced order book management, trade execution, and P&L tracking capabilities.

## Features

- **Advanced Order Book Management**
  - Price-time priority matching engine
  - Support for limit, market, and stop orders
  - Efficient order lookup and management
  - Real-time trade execution

- **Sophisticated P&L Tracking**
  - Position-based P&L calculation
  - Separate realized and unrealized P&L
  - Mark-to-market position valuation
  - Average price tracking
  - Multiple P&L calculation methodologies (aggressor-based and position-based)

- **Multiple Trading Strategies**
  - Market Making Strategy: Quotes both sides with configurable spread
  - Momentum Strategy: Follows short-term price trends
  - Mean Reversion Strategy: Trades mean reversions with configurable window

- **Comprehensive Logging**
  - Detailed trade logs with timestamps
  - Position and P&L tracking per trade
  - Summary statistics and analytics
  - CSV output for further analysis

## Project Structure

- `src/`      - Source code (core logic, engines, parsers)
- `include/`  - Header files (class declarations)
- `data/`     - Sample data files (CSV for orders, trades)
- `tests/`    - Unit and integration tests
- `scripts/`  - Utility scripts (build, run, etc.)

### Key Components
- **Order**: Represents a market order (ID, side, price, quantity, timestamp, type).
- **OrderBook**: Manages buy/sell orders with priority queues for efficient matching.
- **CSVParser**: Loads and parses order data from CSV files.
- **StrategyEngine**: Implements multiple trading strategies with configurable parameters.
- **TradeLogger**: Advanced trade logging with position tracking and P&L calculations.
- **Utils**: Common utilities including timestamp formatting and other helper functions.

## Build Instructions

### Prerequisites
- C++17 compatible compiler (e.g., g++, clang++)
- CMake 3.10 or higher
- Windows, Linux, or macOS

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

### Input Format (orders.csv)
```csv
order_id,side,price,quantity,timestamp
1,BUY,100.23,12,00:00:01-01/01/2023
2,SELL,101.45,7,00:00:02-01/01/2023
```

### Output Format (trades.csv)
```csv
buy_order_id,sell_order_id,price,quantity,timestamp,aggressor_side,realized_pnl,net_position,avg_price
23,4,100.20,6,12:34:56-23/07/2023,BUY,-6.18,-6,100.23
```

## Code Documentation

- All major classes and functions are documented in the header files
- See `include/` directory for detailed API documentation
- Example usage patterns in test files

## Performance

- O(1) best price lookup using priority queues
- O(log n) order insertion and removal
- Efficient position and P&L tracking
- Real-time trade execution and logging

## Contributing

1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

---

For bug reports and feature requests, please open an issue on GitHub. 