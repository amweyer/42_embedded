


#define SET_BIT(REG, bit) REG |= _BV(bit)
#define UNSET_BIT(REG,bit) REG &= ~_BV(bit)
#define TOGGLE_BIT(REG,bit) REG ^= _BV(bit)