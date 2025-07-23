# Include Directory

This directory contains header files for the HFT simulator.

## Core Components

### Order.h
Defines the Order class representing market orders with support for:
- Limit orders
- Market orders
- Stop orders
- Order side (BUY/SELL)
- Price, quantity, and timestamp

### OrderBook.h
Implements the order book with:
- Price-time priority matching
- Priority queues for best price lookup
- Support for multiple order types
- Trade execution and logging

### TradeLogger.h
Advanced trade logging system with:
- Position tracking
- P&L calculations (realized/unrealized)
- Mark-to-market valuation
- CSV output formatting

### StrategyEngine.h
Trading strategy framework with implementations of:
- Market Making Strategy
- Momentum Strategy
- Mean Reversion Strategy

### CSVParser.h
Utilities for parsing order data from CSV files.

### Utils.h
Common utility functions including:
- Timestamp formatting
- Helper functions

## Usage

Include the necessary headers in your source files:
```cpp
#include "Order.h"
#include "OrderBook.h"
#include "TradeLogger.h"
// etc...
```

See individual header files for detailed API documentation and usage examples. 