# Data Directory

This directory contains data files for the HFT simulator.

## Input Files

### orders.csv
Contains the initial orders to load into the order book.

Format:
```csv
order_id,side,price,quantity,timestamp
1,BUY,100.23,12,00:00:01-01/01/2023
2,SELL,101.45,7,00:00:02-01/01/2023
```

Fields:
- `order_id`: Unique identifier for the order
- `side`: BUY or SELL
- `price`: Order price (limit price for limit orders)
- `quantity`: Number of units to trade
- `timestamp`: Order creation time (HH:MM:SS-DD/MM/YYYY)

## Output Files

### trades.csv
Contains executed trades and P&L information.

Format:
```csv
buy_order_id,sell_order_id,price,quantity,timestamp,aggressor_side,realized_pnl,net_position,avg_price
23,4,100.20,6,12:34:56-23/07/2023,BUY,-6.18,-6,100.23
```

Fields:
- `buy_order_id`: ID of the buy order
- `sell_order_id`: ID of the sell order
- `price`: Execution price
- `quantity`: Number of units traded
- `timestamp`: Trade execution time (HH:MM:SS-DD/MM/YYYY)
- `aggressor_side`: Which side initiated the trade (BUY/SELL)
- `realized_pnl`: Cumulative realized P&L after this trade
- `net_position`: Current position after this trade
- `avg_price`: Average position price after this trade

At the end of the file, a trading summary is appended with:
- Aggressor-Based P&L
- Realized P&L
- Unrealized P&L
- Total P&L
- Final Position
- Average Price
- Mark Price 