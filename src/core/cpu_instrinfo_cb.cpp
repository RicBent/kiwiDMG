#include "cpu.h"

const CPU::InstrInfo CPU::instrInfoCB[0x100] =
{
    { 2,  8, "RLC B"        },	// 00
    { 2,  8, "RLC C"        },	// 01
    { 2,  8, "RLC D"        },	// 02
    { 2,  8, "RLC E"        },	// 03
    { 2,  8, "RLC H"        },	// 04
    { 2,  8, "RLC L"        },	// 05
    { 2, 16, "RLC (HL)"     },	// 06
    { 2,  8, "RLC A"        },	// 07
    { 2,  8, "RRC B"        },	// 08
    { 2,  8, "RRC C"        },	// 09
    { 2,  8, "RRC D"        },	// 0A
    { 2,  8, "RRC E"        },	// 0B
    { 2,  8, "RRC H"        },	// 0C
    { 2,  8, "RRC L"        },	// 0D
    { 2, 16, "RRC (HL)"     },	// 0E
    { 2,  8, "RRC A"        },	// 0F
    { 2,  8, "RL B"         },	// 10
    { 2,  8, "RL C"         },	// 11
    { 2,  8, "RL D"         },	// 12
    { 2,  8, "RL E"         },	// 13
    { 2,  8, "RL H"         },	// 14
    { 2,  8, "RL L"         },	// 15
    { 2, 16, "RL (HL)"      },	// 16
    { 2,  8, "RL A"         },	// 17
    { 2,  8, "RR B"         },	// 18
    { 2,  8, "RR C"         },	// 19
    { 2,  8, "RR D"         },	// 1A
    { 2,  8, "RR E"         },	// 1B
    { 2,  8, "RR H"         },	// 1C
    { 2,  8, "RR L"         },	// 1D
    { 2, 16, "RR (HL)"      },	// 1E
    { 2,  8, "RR A"         },	// 1F
    { 2,  8, "SLA B"        },	// 20
    { 2,  8, "SLA C"        },	// 21
    { 2,  8, "SLA D"        },	// 22
    { 2,  8, "SLA E"        },	// 23
    { 2,  8, "SLA H"        },	// 24
    { 2,  8, "SLA L"        },	// 25
    { 2, 16, "SLA (HL)"     },	// 26
    { 2,  8, "SLA A"        },	// 27
    { 2,  8, "SRA B"        },	// 28
    { 2,  8, "SRA C"        },	// 29
    { 2,  8, "SRA D"        },	// 2A
    { 2,  8, "SRA E"        },	// 2B
    { 2,  8, "SRA H"        },	// 2C
    { 2,  8, "SRA L"        },	// 2D
    { 2, 16, "SRA (HL)"     },	// 2E
    { 2,  8, "SRA A"        },	// 2F
    { 2,  8, "SWAP B"       },	// 30
    { 2,  8, "SWAP C"       },	// 31
    { 2,  8, "SWAP D"       },	// 32
    { 2,  8, "SWAP E"       },	// 33
    { 2,  8, "SWAP H"       },	// 34
    { 2,  8, "SWAP L"       },	// 35
    { 2, 16, "SWAP (HL)"    },	// 36
    { 2,  8, "SWAP A"       },	// 37
    { 2,  8, "SRL B"        },	// 38
    { 2,  8, "SRL C"        },	// 39
    { 2,  8, "SRL D"        },	// 3A
    { 2,  8, "SRL E"        },	// 3B
    { 2,  8, "SRL H"        },	// 3C
    { 2,  8, "SRL L"        },	// 3D
    { 2, 16, "SRL (HL)"     },	// 3E
    { 2,  8, "SRL A"        },	// 3F
    { 2,  8, "BIT 0,B"      },	// 40
    { 2,  8, "BIT 0,C"      },	// 41
    { 2,  8, "BIT 0,D"      },	// 42
    { 2,  8, "BIT 0,E"      },	// 43
    { 2,  8, "BIT 0,H"      },	// 44
    { 2,  8, "BIT 0,L"      },	// 45
    { 2, 16, "BIT 0,(HL)"   },	// 46
    { 2,  8, "BIT 0,A"      },	// 47
    { 2,  8, "BIT 1,B"      },	// 48
    { 2,  8, "BIT 1,C"      },	// 49
    { 2,  8, "BIT 1,D"      },	// 4A
    { 2,  8, "BIT 1,E"      },	// 4B
    { 2,  8, "BIT 1,H"      },	// 4C
    { 2,  8, "BIT 1,L"      },	// 4D
    { 2, 16, "BIT 1,(HL)"   },	// 4E
    { 2,  8, "BIT 1,A"      },	// 4F
    { 2,  8, "BIT 2,B"      },	// 50
    { 2,  8, "BIT 2,C"      },	// 51
    { 2,  8, "BIT 2,D"      },	// 52
    { 2,  8, "BIT 2,E"      },	// 53
    { 2,  8, "BIT 2,H"      },	// 54
    { 2,  8, "BIT 2,L"      },	// 55
    { 2, 16, "BIT 2,(HL)"   },	// 56
    { 2,  8, "BIT 2,A"      },	// 57
    { 2,  8, "BIT 3,B"      },	// 58
    { 2,  8, "BIT 3,C"      },	// 59
    { 2,  8, "BIT 3,D"      },	// 5A
    { 2,  8, "BIT 3,E"      },	// 5B
    { 2,  8, "BIT 3,H"      },	// 5C
    { 2,  8, "BIT 3,L"      },	// 5D
    { 2, 16, "BIT 3,(HL)"   },	// 5E
    { 2,  8, "BIT 3,A"      },	// 5F
    { 2,  8, "BIT 4,B"      },	// 60
    { 2,  8, "BIT 4,C"      },	// 61
    { 2,  8, "BIT 4,D"      },	// 62
    { 2,  8, "BIT 4,E"      },	// 63
    { 2,  8, "BIT 4,H"      },	// 64
    { 2,  8, "BIT 4,L"      },	// 65
    { 2, 16, "BIT 4,(HL)"   },	// 66
    { 2,  8, "BIT 4,A"      },	// 67
    { 2,  8, "BIT 5,B"      },	// 68
    { 2,  8, "BIT 5,C"      },	// 69
    { 2,  8, "BIT 5,D"      },	// 6A
    { 2,  8, "BIT 5,E"      },	// 6B
    { 2,  8, "BIT 5,H"      },	// 6C
    { 2,  8, "BIT 5,L"      },	// 6D
    { 2, 16, "BIT 5,(HL)"   },	// 6E
    { 2,  8, "BIT 5,A"      },	// 6F
    { 2,  8, "BIT 6,B"      },	// 70
    { 2,  8, "BIT 6,C"      },	// 71
    { 2,  8, "BIT 6,D"      },	// 72
    { 2,  8, "BIT 6,E"      },	// 73
    { 2,  8, "BIT 6,H"      },	// 74
    { 2,  8, "BIT 6,L"      },	// 75
    { 2, 16, "BIT 6,(HL)"   },	// 76
    { 2,  8, "BIT 6,A"      },	// 77
    { 2,  8, "BIT 7,B"      },	// 78
    { 2,  8, "BIT 7,C"      },	// 79
    { 2,  8, "BIT 7,D"      },	// 7A
    { 2,  8, "BIT 7,E"      },	// 7B
    { 2,  8, "BIT 7,H"      },	// 7C
    { 2,  8, "BIT 7,L"      },	// 7D
    { 2, 16, "BIT 7,(HL)"   },	// 7E
    { 2,  8, "BIT 7,A"      },	// 7F
    { 2,  8, "RES 0,B"      },	// 80
    { 2,  8, "RES 0,C"      },	// 81
    { 2,  8, "RES 0,D"      },	// 82
    { 2,  8, "RES 0,E"      },	// 83
    { 2,  8, "RES 0,H"      },	// 84
    { 2,  8, "RES 0,L"      },	// 85
    { 2, 16, "RES 0,(HL)"   },	// 86
    { 2,  8, "RES 0,A"      },	// 87
    { 2,  8, "RES 1,B"      },	// 88
    { 2,  8, "RES 1,C"      },	// 89
    { 2,  8, "RES 1,D"      },	// 8A
    { 2,  8, "RES 1,E"      },	// 8B
    { 2,  8, "RES 1,H"      },	// 8C
    { 2,  8, "RES 1,L"      },	// 8D
    { 2, 16, "RES 1,(HL)"   },	// 8E
    { 2,  8, "RES 1,A"      },	// 8F
    { 2,  8, "RES 2,B"      },	// 90
    { 2,  8, "RES 2,C"      },	// 91
    { 2,  8, "RES 2,D"      },	// 92
    { 2,  8, "RES 2,E"      },	// 93
    { 2,  8, "RES 2,H"      },	// 94
    { 2,  8, "RES 2,L"      },	// 95
    { 2, 16, "RES 2,(HL)"   },	// 96
    { 2,  8, "RES 2,A"      },	// 97
    { 2,  8, "RES 3,B"      },	// 98
    { 2,  8, "RES 3,C"      },	// 99
    { 2,  8, "RES 3,D"      },	// 9A
    { 2,  8, "RES 3,E"      },	// 9B
    { 2,  8, "RES 3,H"      },	// 9C
    { 2,  8, "RES 3,L"      },	// 9D
    { 2, 16, "RES 3,(HL)"   },	// 9E
    { 2,  8, "RES 3,A"      },	// 9F
    { 2,  8, "RES 4,B"      },	// A0
    { 2,  8, "RES 4,C"      },	// A1
    { 2,  8, "RES 4,D"      },	// A2
    { 2,  8, "RES 4,E"      },	// A3
    { 2,  8, "RES 4,H"      },	// A4
    { 2,  8, "RES 4,L"      },	// A5
    { 2, 16, "RES 4,(HL)"   },	// A6
    { 2,  8, "RES 4,A"      },	// A7
    { 2,  8, "RES 5,B"      },	// A8
    { 2,  8, "RES 5,C"      },	// A9
    { 2,  8, "RES 5,D"      },	// AA
    { 2,  8, "RES 5,E"      },	// AB
    { 2,  8, "RES 5,H"      },	// AC
    { 2,  8, "RES 5,L"      },	// AD
    { 2, 16, "RES 5,(HL)"   },	// AE
    { 2,  8, "RES 5,A"      },	// AF
    { 2,  8, "RES 6,B"      },	// B0
    { 2,  8, "RES 6,C"      },	// B1
    { 2,  8, "RES 6,D"      },	// B2
    { 2,  8, "RES 6,E"      },	// B3
    { 2,  8, "RES 6,H"      },	// B4
    { 2,  8, "RES 6,L"      },	// B5
    { 2, 16, "RES 6,(HL)"   },	// B6
    { 2,  8, "RES 6,A"      },	// B7
    { 2,  8, "RES 7,B"      },	// B8
    { 2,  8, "RES 7,C"      },	// B9
    { 2,  8, "RES 7,D"      },	// BA
    { 2,  8, "RES 7,E"      },	// BB
    { 2,  8, "RES 7,H"      },	// BC
    { 2,  8, "RES 7,L"      },	// BD
    { 2, 16, "RES 7,(HL)"   },	// BE
    { 2,  8, "RES 7,A"      },	// BF
    { 2,  8, "SET 0,B"      },	// C0
    { 2,  8, "SET 0,C"      },	// C1
    { 2,  8, "SET 0,D"      },	// C2
    { 2,  8, "SET 0,E"      },	// C3
    { 2,  8, "SET 0,H"      },	// C4
    { 2,  8, "SET 0,L"      },	// C5
    { 2, 16, "SET 0,(HL)"   },	// C6
    { 2,  8, "SET 0,A"      },	// C7
    { 2,  8, "SET 1,B"      },	// C8
    { 2,  8, "SET 1,C"      },	// C9
    { 2,  8, "SET 1,D"      },	// CA
    { 2,  8, "SET 1,E"      },	// CB
    { 2,  8, "SET 1,H"      },	// CC
    { 2,  8, "SET 1,L"      },	// CD
    { 2, 16, "SET 1,(HL)"   },	// CE
    { 2,  8, "SET 1,A"      },	// CF
    { 2,  8, "SET 2,B"      },	// D0
    { 2,  8, "SET 2,C"      },	// D1
    { 2,  8, "SET 2,D"      },	// D2
    { 2,  8, "SET 2,E"      },	// D3
    { 2,  8, "SET 2,H"      },	// D4
    { 2,  8, "SET 2,L"      },	// D5
    { 2, 16, "SET 2,(HL)"   },	// D6
    { 2,  8, "SET 2,A"      },	// D7
    { 2,  8, "SET 3,B"      },	// D8
    { 2,  8, "SET 3,C"      },	// D9
    { 2,  8, "SET 3,D"      },	// DA
    { 2,  8, "SET 3,E"      },	// DB
    { 2,  8, "SET 3,H"      },	// DC
    { 2,  8, "SET 3,L"      },	// DD
    { 2, 16, "SET 3,(HL)"   },	// DE
    { 2,  8, "SET 3,A"      },	// DF
    { 2,  8, "SET 4,B"      },	// E0
    { 2,  8, "SET 4,C"      },	// E1
    { 2,  8, "SET 4,D"      },	// E2
    { 2,  8, "SET 4,E"      },	// E3
    { 2,  8, "SET 4,H"      },	// E4
    { 2,  8, "SET 4,L"      },	// E5
    { 2, 16, "SET 4,(HL)"   },	// E6
    { 2,  8, "SET 4,A"      },	// E7
    { 2,  8, "SET 5,B"      },	// E8
    { 2,  8, "SET 5,C"      },	// E9
    { 2,  8, "SET 5,D"      },	// EA
    { 2,  8, "SET 5,E"      },	// EB
    { 2,  8, "SET 5,H"      },	// EC
    { 2,  8, "SET 5,L"      },	// ED
    { 2, 16, "SET 5,(HL)"   },	// EE
    { 2,  8, "SET 5,A"      },	// EF
    { 2,  8, "SET 6,B"      },	// F0
    { 2,  8, "SET 6,C"      },	// F1
    { 2,  8, "SET 6,D"      },	// F2
    { 2,  8, "SET 6,E"      },	// F3
    { 2,  8, "SET 6,H"      },	// F4
    { 2,  8, "SET 6,L"      },	// F5
    { 2, 16, "SET 6,(HL)"   },	// F6
    { 2,  8, "SET 6,A"      },	// F7
    { 2,  8, "SET 7,B"      },	// F8
    { 2,  8, "SET 7,C"      },	// F9
    { 2,  8, "SET 7,D"      },	// FA
    { 2,  8, "SET 7,E"      },	// FB
    { 2,  8, "SET 7,H"      },	// FC
    { 2,  8, "SET 7,L"      },	// FD
    { 2, 16, "SET 7,(HL)"   },	// FE
    { 2,  8, "SET 7,A"      }	// FF
};