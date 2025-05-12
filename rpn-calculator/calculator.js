// Define a class to simulate stack
class Stack {
    constructor() {
        this.stack = [];
    }

    // Push an element into the stack
    push(e) {
        this.stack.push(e);
    }

    // Pop an element off the stack
    pop() {
        if (this.isEmpty()) {
            throw new Error("Stack underflow");
        }
        return this.stack.pop();
    }

    // Peek at the top element
    peek() {
        if (this.isEmpty()) {
            throw new Error("Stack is empty");
        }
        return this.stack[this.stack.length - 1]
    }

    // Check if the stack is empty
    isEmpty() {
        return this.stack.length === 0;
    }

    // Get the size of the stack
    size() {
        return this.stack.length;
    }
}

// Define a class to simulate symbol table
class SymbolTable {
    constructor() {
        this.table = {};
    }

    // Insert element to the table
    insert(key, value) {
        this.table[key] = value;
    }

    // Check if the key is in the table
    hasKey(key) {
        return key in this.table;
    }

    // Search the value of key in the table
    search(key) {
        if (this.hasKey(key)) {
            return this.table[key];
        } else {
            throw new Error(`Key '${key}' not found`);
        }
    }

    // Delete key from the table
    delete(key) {
        if (this.hasKey(key)) {
            delete this.table[key];
        }
    }
}

const T = new SymbolTable();  // Create a symbole talbe to store variables and values

function Main(input) {
    const S = new Stack();  // Create a stack for the process
    let i = 0;  // The index of position to read

    while (i < input.length) {
        if (input[i] === " ") {  // Skip the space
            i++;
        } else if (isNum(input[i])) {  // Read numbers
            const [e, nextIndex] = ReadNum(input, i);
            S.push(e);
            i = nextIndex;
        } else if (i < input.length - 1 && input[i] === "-" && isNum(input[i + 1])) {  // Handle negative number
            const [e, nextIndex] = ReadNum(input, i + 1);
            S.push(-e);
            i = nextIndex;
        } else if (isVar(input[i])) {  // Push the variable name onto stack
            S.push(input[i])
            i++;
        } else if (isOp(input[i])) {  // Hanlde operators
            HandleOp(input[i], S, T);
            i++;
        } else if (isFunc(input, i)) {  // Handle functions
            const func = input.substring(i, i + (input.substring(i, i + 4) === 'sqrt' ? 4 : 3));
            HandleFunc(func, S, T)
            i += func.length;
        } else if (input[i] === "=") {  // Handle assignment
            HandleAsg(S, T);
            i++;
        } else {
            throw new Error(`Invalid input: '${input[i]}' at index ${i}`);
        }
    }
    if (S.size() > 1) {  // The result should be one number, or empty if it is assignment
        throw new Error("Invalid expression");
    }
    return S.isEmpty() ? null : S.peek();  // Return the result
}

// Read number, including decimal
function ReadNum(input, startIndex) {
    let e = 0;
    let i = startIndex;
    let isDecimal = false;  // Check if the number is decimal
    let decimalPlace = 1;  // The decimal place of current digit

    while (i < input.length && (isNum(input[i]) || input[i] === ".")) {  // Check if current character is a number
        if (input[i] === ".") {  // If met decimal point, numbers after will be as decimal
            if (isDecimal) {  // Handle multiple decimal point issue
                throw new Error("Invalid value: multiple decimal point")
            }
            isDecimal = true;
        } else {
            if (isDecimal) {  // If it is decimal, use decimalPlace to calculate
                e += parseInt(input[i], 10) / (10 ** decimalPlace);
                decimalPlace++;
            } else {  // If it is integer, use another way
                e = e * 10 + parseInt(input[i], 10);
            }
        }
        i++;  // Read the next character
    }
    return [e, i];  // Return the result number, and the index of next position to read
}

// Handle operators
function HandleOp(op, S, T) {
    if (S.size() < 2) {  // Currently all operators here need 2 operands
        throw new Error(`Insufficient operands for operator '${op}'`);
    }
    const b = ConvertVar(T, S.pop());  // The first popped element will be the second operand
    const a = ConvertVar(T, S.pop());
    let e;  // The result
    switch (op) {
        case "+":
            e = a + b;
            break;
        case "-":
            e = a - b;
            break;
        case "*":
            e = a * b;
            break;
        case "/":
            if (b === 0) {
                throw new Error("Divided by 0");
            }
            e = a / b;
            break;
        case "^":
            e = a ** b;
            break;
        case "%":
            if (b === 0) {
                throw new Error("Modulo by 0");
            }
            e = a % b;
            break;
        default:
            throw new Error(`Invalid operator '${op}'`);
    }
    S.push(e);
}

// Handle trigonometric functions
function HandleFunc(func, S, T) {
    if (S.size() < 1) {  // Currently all functions here need 1 operand
        throw new Error(`Insufficient operands for operator '${func}'`);
    }
    const a = ConvertVar(T, S.pop());
    let e;

    switch (func) {
        case 'sin':
            e = Math.sin(a);
            break;
        case 'cos':
            e = Math.cos(a);
            break;
        case 'tan':
            e = Math.tan(a);
            break;
        case 'log':
            if (a <= 0) {
                throw new Error("Logarithm undefined for non-positive number")
            }
            e = Math.log(a);
            break;
        case 'sqrt':
            if (a < 0) {
                throw new Error("Square root undefined for non-positive number")
            }
            e = Math.sqrt(a);
            break;
        default:
            throw new Error(`Unknown function '${func}'`)
    }
    S.push(e);
}

// Handle assignment expression
function HandleAsg(S, T) {
    if (S.size() < 2) {  // Need 2 operands to make assighment
        throw new Error("Insufficient operands for assignment")
    }
    const value = ConvertVar(T, S.pop());  // The key is the first operand, and the value is the second
    const key = S.pop();
    if (isVar(key)) {
        T.insert(key, value);
        console.log(`Symbol table after assignment: ${JSON.stringify(T.table)}`);  // Print the symbol table for reference
    } else {
        throw new Error(`Invalid assignment: '${key}' = '${value}'`);
    }
}

// Convert variable to number
function ConvertVar(T, key) {
    if (isVar(key)) {  // If met variable name, check if corresponding value exists
        if (T.hasKey(key)) {
            return T.search(key);  // Return the value if so
        }
        throw new Error(`Undefined variable: '${key}'`);
    } else if (typeof key === 'number') {  // If met number, just return it back
        return key;
    }
    throw new Error(`Invalid operand: '${key}'`);
}

// Utility functions
function isNum(char) {  // Check if it is a number
    return !isNaN(parseInt(char, 10));
}

function isVar(char) {  // Check if it is a variable name(uppercase letter)
    return char >= 'A' && char <= 'Z';
}

function isOp(char) {  // Check if it is an operator
    return ['+', '-', '*', '/', '^', '%'].includes(char);
}

function isFunc(input, i) {  // Check if it is a function
    const str3 = input.substring(i, i + 3);
    const str4 = input.substring(i, i + 4);
    return ['sin', 'cos', 'tan', 'log'].includes(str3) || ['sqrt'].includes(str4);
}

// Link to console
import readline from 'readline';

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
});

console.log("Enter your expression:");
rl.on("line", (line) => {
    try {
        const result = Main(line.trim());
        console.log("Result:", result);
    } catch (error) {
        console.error("Error:", error.message);
    }
    console.log("\nEnter next expression:");
});

export {
    Stack,
    SymbolTable,
    Main,
    HandleOp,
    HandleFunc,
    HandleAsg
};