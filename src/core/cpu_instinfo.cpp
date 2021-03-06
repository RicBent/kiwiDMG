#include "cpu.h"

const CPU::InstrInfo CPU::instrInfo[0x100] =
{
    { 1,  4, "NOP"          },  // 00
    { 3, 12, "LD BC,d16"    },	// 01
    { 1,  8, "LD (BC),A"    },	// 02
    { 1,  8, "INC BC"       },	// 03
    { 1,  4, "INC B"        },	// 04
    { 1,  4, "DEC B"        },	// 05
    { 2,  8, "LD B,d8"      },	// 06
    { 1,  4, "RLCA"         },	// 07
    { 3, 20, "LD (a16),SP"  },	// 08
    { 1,  8, "ADD HL,BC"    },	// 09
    { 1,  8, "LD A,(BC)"    },	// 0A
    { 1,  8, "DEC BC"       },	// 0B
    { 1,  4, "INC C"        },	// 0C
    { 1,  4, "DEC C"        },	// 0D
    { 2,  8, "LD C,d8"      },	// 0E
    { 1,  4, "RRCA"         },	// 0F
    { 2,  4, "STOP 0"       },	// 10
    { 3, 12, "LD DE,d16"    },	// 11
    { 1,  8, "LD (DE),A"    },	// 12
    { 1,  8, "INC DE"       },	// 13
    { 1,  4, "INC D"        },	// 14
    { 1,  4, "DEC D"        },	// 15
    { 2,  8, "LD D,d8"      },	// 16
    { 1,  4, "RLA"          },	// 17
    { 2, 12, "JR r8"        },	// 18
    { 1,  8, "ADD HL,DE"    },	// 19
    { 1,  8, "LD A,(DE)"    },	// 1A
    { 1,  8, "DEC DE"       },	// 1B
    { 1,  4, "INC E"        },	// 1C
    { 1,  4, "DEC E"        },	// 1D
    { 2,  8, "LD E,d8"      },	// 1E
    { 1,  4, "RRA"          },	// 1F
    { 2,  8, "JR NZ,r8"     },	// 20
    { 3, 12, "LD HL,d16"    },	// 21
    { 1,  8, "LD (HL+),A"   },	// 22
    { 1,  8, "INC HL"       },	// 23
    { 1,  4, "INC H"        },	// 24
    { 1,  4, "DEC H"        },	// 25
    { 2,  8, "LD H,d8"      },	// 26
    { 1,  4, "DAA"          },	// 27
    { 2,  8, "JR Z,r8"      },	// 28
    { 1,  8, "ADD HL,HL"    },	// 29
    { 1,  8, "LD A,(HL+)"   },	// 2A
    { 1,  8, "DEC HL"       },	// 2B
    { 1,  4, "INC L"        },	// 2C
    { 1,  4, "DEC L"        },	// 2D
    { 2,  8, "LD L,d8"      },	// 2E
    { 1,  4, "CPL"          },	// 2F
    { 2,  8, "JR NC,r8"     },	// 30
    { 3, 12, "LD SP,d16"    },	// 31
    { 1,  8, "LD (HL-),A"   },	// 32
    { 1,  8, "INC SP"       },	// 33
    { 1, 12, "INC (HL)"     },	// 34
    { 1, 12, "DEC (HL)"     },	// 35
    { 2, 12, "LD (HL),d8"   },	// 36
    { 1,  4, "SCF"          },	// 37
    { 2,  8, "JR C,r8"      },	// 38
    { 1,  8, "ADD HL,SP"    },	// 39
    { 1,  8, "LD A,(HL-)"   },	// 3A
    { 1,  8, "DEC SP"       },	// 3B
    { 1,  4, "INC A"        },	// 3C
    { 1,  4, "DEC A"        },	// 3D
    { 2,  8, "LD A,d8"      },	// 3E
    { 1,  4, "CCF"          },	// 3F
    { 1,  4, "LD B,B"       },	// 40
    { 1,  4, "LD B,C"       },	// 41
    { 1,  4, "LD B,D"       },	// 42
    { 1,  4, "LD B,E"       },	// 43
    { 1,  4, "LD B,H"       },	// 44
    { 1,  4, "LD B,L"       },	// 45
    { 1,  8, "LD B,(HL)"    },	// 46
    { 1,  4, "LD B,A"       },	// 47
    { 1,  4, "LD C,B"       },	// 48
    { 1,  4, "LD C,C"       },	// 49
    { 1,  4, "LD C,D"       },	// 4A
    { 1,  4, "LD C,E"       },	// 4B
    { 1,  4, "LD C,H"       },	// 4C
    { 1,  4, "LD C,L"       },	// 4D
    { 1,  8, "LD C,(HL)"    },	// 4E
    { 1,  4, "LD C,A"       },	// 4F
    { 1,  4, "LD D,B"       },	// 50
    { 1,  4, "LD D,C"       },	// 51
    { 1,  4, "LD D,D"       },	// 52
    { 1,  4, "LD D,E"       },	// 53
    { 1,  4, "LD D,H"       },	// 54
    { 1,  4, "LD D,L"       },	// 55
    { 1,  8, "LD D,(HL)"    },	// 56
    { 1,  4, "LD D,A"       },	// 57
    { 1,  4, "LD E,B"       },	// 58
    { 1,  4, "LD E,C"       },	// 59
    { 1,  4, "LD E,D"       },	// 5A
    { 1,  4, "LD E,E"       },	// 5B
    { 1,  4, "LD E,H"       },	// 5C
    { 1,  4, "LD E,L"       },	// 5D
    { 1,  8, "LD E,(HL)"    },	// 5E
    { 1,  4, "LD E,A"       },	// 5F
    { 1,  4, "LD H,B"       },	// 60
    { 1,  4, "LD H,C"       },	// 61
    { 1,  4, "LD H,D"       },	// 62
    { 1,  4, "LD H,E"       },	// 63
    { 1,  4, "LD H,H"       },	// 64
    { 1,  4, "LD H,L"       },	// 65
    { 1,  8, "LD H,(HL)"    },	// 66
    { 1,  4, "LD H,A"       },	// 67
    { 1,  4, "LD L,B"       },	// 68
    { 1,  4, "LD L,C"       },	// 69
    { 1,  4, "LD L,D"       },	// 6A
    { 1,  4, "LD L,E"       },	// 6B
    { 1,  4, "LD L,H"       },	// 6C
    { 1,  4, "LD L,L"       },	// 6D
    { 1,  8, "LD L,(HL)"    },	// 6E
    { 1,  4, "LD L,A"       },	// 6F
    { 1,  8, "LD (HL),B"    },	// 70
    { 1,  8, "LD (HL),C"    },	// 71
    { 1,  8, "LD (HL),D"    },	// 72
    { 1,  8, "LD (HL),E"    },	// 73
    { 1,  8, "LD (HL),H"    },	// 74
    { 1,  8, "LD (HL),L"    },	// 75
    { 1,  4, "HALT"         },	// 76
    { 1,  8, "LD (HL),A"    },	// 77
    { 1,  4, "LD A,B"       },	// 78
    { 1,  4, "LD A,C"       },	// 79
    { 1,  4, "LD A,D"       },	// 7A
    { 1,  4, "LD A,E"       },	// 7B
    { 1,  4, "LD A,H"       },	// 7C
    { 1,  4, "LD A,L"       },	// 7D
    { 1,  8, "LD A,(HL)"    },	// 7E
    { 1,  4, "LD A,A"       },	// 7F
    { 1,  4, "ADD A,B"      },	// 80
    { 1,  4, "ADD A,C"      },	// 81
    { 1,  4, "ADD A,D"      },	// 82
    { 1,  4, "ADD A,E"      },	// 83
    { 1,  4, "ADD A,H"      },	// 84
    { 1,  4, "ADD A,L"      },	// 85
    { 1,  8, "ADD A,(HL)"   },	// 86
    { 1,  4, "ADD A,A"      },	// 87
    { 1,  4, "ADC A,B"      },	// 88
    { 1,  4, "ADC A,C"      },	// 89
    { 1,  4, "ADC A,D"      },	// 8A
    { 1,  4, "ADC A,E"      },	// 8B
    { 1,  4, "ADC A,H"      },	// 8C
    { 1,  4, "ADC A,L"      },	// 8D
    { 1,  8, "ADC A,(HL)"   },	// 8E
    { 1,  4, "ADC A,A"      },	// 8F
    { 1,  4, "SUB B"        },	// 90
    { 1,  4, "SUB C"        },	// 91
    { 1,  4, "SUB D"        },	// 92
    { 1,  4, "SUB E"        },	// 93
    { 1,  4, "SUB H"        },	// 94
    { 1,  4, "SUB L"        },	// 95
    { 1,  8, "SUB (HL)"     },	// 96
    { 1,  4, "SUB A"        },	// 97
    { 1,  4, "SBC A,B"      },	// 98
    { 1,  4, "SBC A,C"      },	// 99
    { 1,  4, "SBC A,D"      },	// 9A
    { 1,  4, "SBC A,E"      },	// 9B
    { 1,  4, "SBC A,H"      },	// 9C
    { 1,  4, "SBC A,L"      },	// 9D
    { 1,  8, "SBC A,(HL)"   },	// 9E
    { 1,  4, "SBC A,A"      },	// 9F
    { 1,  4, "AND B"        },	// A0
    { 1,  4, "AND C"        },	// A1
    { 1,  4, "AND D"        },	// A2
    { 1,  4, "AND E"        },	// A3
    { 1,  4, "AND H"        },	// A4
    { 1,  4, "AND L"        },	// A5
    { 1,  8, "AND (HL)"     },	// A6
    { 1,  4, "AND A"        },	// A7
    { 1,  4, "XOR B"        },	// A8
    { 1,  4, "XOR C"        },	// A9
    { 1,  4, "XOR D"        },	// AA
    { 1,  4, "XOR E"        },	// AB
    { 1,  4, "XOR H"        },	// AC
    { 1,  4, "XOR L"        },	// AD
    { 1,  8, "XOR (HL)"     },	// AE
    { 1,  4, "XOR A"        },	// AF
    { 1,  4, "OR B"         },	// B0
    { 1,  4, "OR C"         },	// B1
    { 1,  4, "OR D"         },	// B2
    { 1,  4, "OR E"         },	// B3
    { 1,  4, "OR H"         },	// B4
    { 1,  4, "OR L"         },	// B5
    { 1,  8, "OR (HL)"      },	// B6
    { 1,  4, "OR A"         },	// B7
    { 1,  4, "CP B"         },	// B8
    { 1,  4, "CP C"         },	// B9
    { 1,  4, "CP D"         },	// BA
    { 1,  4, "CP E"         },	// BB
    { 1,  4, "CP H"         },	// BC
    { 1,  4, "CP L"         },	// BD
    { 1,  8, "CP (HL)"      },	// BE
    { 1,  4, "CP A"         },	// BF
    { 1,  8, "RET NZ"       },	// C0
    { 1, 12, "POP BC"       },	// C1
    { 3,  0, "JP NZ,a16"    },	// C2
    { 3, 16, "JP a16"       },	// C3
    { 3, 12, "CALL NZ,a16"  },	// C4
    { 1, 16, "PUSH BC"      },	// C5
    { 2,  8, "ADD A,d8"     },	// C6
    { 1, 16, "RST 00H"      },	// C7
    { 1,  8, "RET Z"        },	// C8
    { 1, 16, "RET"          },	// C9
    { 3,  0, "JP Z,a16"     },	// CA
    { 1,  4, "PREFIX CB"    },	// CB
    { 3, 12, "CALL Z,a16"   },	// CC
    { 3, 24, "CALL a16"     },	// CD
    { 2,  8, "ADC A,d8"     },	// CE
    { 1, 16, "RST 08H"      },	// CF
    { 1,  8, "RET NC"       },	// D0
    { 1, 12, "POP DE"       },	// D1
    { 3,  0, "JP NC,a16"    },	// D2
    { 1,  4, "INVALID"      },	// D3
    { 3, 12, "CALL NC,a16"  },	// D4
    { 1, 16, "PUSH DE"      },	// D5
    { 2,  8, "SUB d8"       },	// D6
    { 1, 16, "RST 10H"      },	// D7
    { 1,  8, "RET C"        },	// D8
    { 1, 16, "RETI"         },	// D9
    { 3,  0, "JP C,a16"     },	// DA
    { 1,  4, "INVALID"      },	// DB
    { 3, 12, "CALL C,a16"   },	// DC
    { 1,  4, "INVALID"      },	// DD
    { 2,  8, "SBC A,d8"     },	// DE
    { 1, 16, "RST 18H"      },	// DF
    { 2, 12, "LDH (a8),A"   },	// E0
    { 1, 12, "POP HL"       },	// E1
    { 1,  8, "LD (C),A"     },	// E2
    { 1,  4, "INVALID"      },	// E3
    { 1,  4, "INVALID"      },	// E4
    { 1, 16, "PUSH HL"      },	// E5
    { 2,  8, "AND d8"       },	// E6
    { 1, 16, "RST 20H"      },	// E7
    { 2, 16, "ADD SP,r8"    },	// E8
    { 1,  4, "JP (HL)"      },	// E9
    { 3, 16, "LD (a16),A"   },	// EA
    { 1,  4, "INVALID"      },	// EB
    { 1,  4, "INVALID"      },	// EC
    { 1,  4, "INVALID"      },	// ED
    { 2,  8, "XOR d8"       },	// EE
    { 1, 16, "RST 28H"      },	// EF
    { 2, 12, "LDH A,(a8)"   },	// F0
    { 1, 12, "POP AF"       },	// F1
    { 2,  8, "LD A,(C)"     },	// F2
    { 1,  4, "DI"           },	// F3
    { 1,  4, "INVALID"      },	// F4
    { 1, 16, "PUSH AF"      },	// F5
    { 2,  8, "OR d8"        },	// F6
    { 1, 16, "RST 30H"      },	// F7
    { 2, 12, "LD HL,SP+r8"  },	// F8
    { 1,  8, "LD SP,HL"     },	// F9
    { 3, 16, "LD A,(a16)"   },	// FA
    { 1,  4, "EI"           },	// FB
    { 1,  4, "INVALID"      },	// FC
    { 1,  4, "INVALID"      },	// FD
    { 2,  8, "CP d8"        },	// FE
    { 1, 16, "RST 38H"      }	// FF
};
