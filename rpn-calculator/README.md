# RPN Calculator with Variables

A stack-based calculator using Reverse Polish Notation (RPN) that supports:
- Basic arithmetic (+, -, *, /, ^, %)
- Math functions (sin, cos, tan, log, sqrt)
- Variable assignments
- Decimal and negative numbers

## Usage

```bash
npm install   # Install dependencies
npm start     # Run calculator
npm test      # Run tests
```

Example expressions:
```
2 3 +           # 5 (2 + 3)
5 2 *           # 10 (5 × 2)
X 5 =           # Assigns 5 to X
X 3 +           # 8 (5 + 3)
16 sqrt         # 4 (√16)
3 4 + 5 *       # 35 ((3 + 4) × 5)
```

## RPN Format
- Numbers and variables are pushed onto stack
- Operators/functions process stack values
- Write operators after their operands