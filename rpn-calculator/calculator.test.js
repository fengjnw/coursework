import { expect } from 'chai';
import { Stack, SymbolTable, Main } from './calculator.js';

describe('RPN Calculator', () => {
    let T;

    beforeEach(() => {
        T = new SymbolTable();  // Reset symbol table before each test
    });

    describe('Basic Operations', () => {
        it('should perform addition', () => {
            expect(Main('2 3 +')).to.equal(5);
        });

        it('should perform subtraction', () => {
            expect(Main('5 3 -')).to.equal(2);
        });

        it('should perform multiplication', () => {
            expect(Main('4 2 *')).to.equal(8);
        });

        it('should perform division', () => {
            expect(Main('8 2 /')).to.equal(4);
        });
    });

    describe('Advanced Operations', () => {
        it('should handle negative numbers', () => {
            expect(Main('-5 2 *')).to.equal(-10);
        });

        it('should handle decimal numbers', () => {
            expect(Main('2.5 2 +')).to.equal(4.5);
        });

        it('should calculate power', () => {
            expect(Main('2 3 ^')).to.equal(8);
        });
    });

    describe('Mathematical Functions', () => {
        it('should calculate sine', () => {
            expect(Main('0 sin')).to.be.closeTo(0, 0.0001);
        });

        it('should calculate square root', () => {
            expect(Main('16 sqrt')).to.equal(4);
        });
    });

    describe('Variable Operations', () => {
        it('should handle variable assignment and usage', () => {
            Main('X 5 =');
            expect(Main('X 3 +')).to.equal(8);
        });
    });

    describe('Error Handling', () => {
        it('should throw error for insufficient operands', () => {
            expect(() => Main('+')).to.throw('Insufficient operands');
        });

        it('should throw error for division by zero', () => {
            expect(() => Main('5 0 /')).to.throw('Divided by 0');
        });
    });
});