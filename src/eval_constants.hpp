#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(201, 455);
inline const PParam KNIGHT_MAT = S(911, 1335);
inline const PParam BISHOP_MAT = S(952, 1396);
inline const PParam ROOK_MAT   = S(1136, 2199);
inline const PParam QUEEN_MAT  = S(2704, 3455);
inline const PParam TEMPO_VAL  = S(65, 23);

inline const PParam BISHOP_XRAY_PAWNS   = S(-15, 15);
inline const PParam BISHOP_PAIR_VAL   = S(59, 233);
inline const PParam ROOK_OPEN_VAL     = S(123, -14);
inline const PParam ROOK_SEMIOPEN_VAL = S(46, 33);
inline const PParam MINOR_BEHIND_PAWN = S(14, 47);
inline const PParam RESTRICTED_SQUARES = S(-25, -7);

inline const PParam DOUBLED_PAWN_VAL = S(-17, -82);
inline const PParam ISOLATED_PAWN_VAL = S(-19, -26);

inline const PParam POTENTIAL_CHECKER_VAL = S(-51, -26);
inline const PParam OUTPOST_KNIGHT_VAL    = S(56, 61);
inline const PParam OUTPOST_BISHOP_VAL    = S(54, 47);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(40, 6);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(50, -20);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(25, 57);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(61, -39);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(17, 2), S(35, 32), S(64, 60), S(129, 177), S(487, 197), S(470, 504),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(48, 41), S(45, 39), S(64, 70), S(115, 175), S(504, 63),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-100, -237), S(-99, -202), S(-74, -71), S(-29, 55), S(78, 233), S(250, 343),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(29, -42), S(19, 6), S(10, 41), S(10, 118), S(91, 229), S(165, 426),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(15, -24), S(1, 18), S(-4, -16), S(-2, -46), S(-16, -142), S(-231, -263),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(16, 219), S(-11, 186), S(-7, 103), S(8, 55), S(22, 53), S(59, 50), S(52, 38),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-229, -57), S(-0, 30), S(0, 104), S(39, 132), S(56, 164), S(67, 171), S(39, 174),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-111, -303), S(-45, -87), S(-14, 12), S(11, 54), S(42, 76), S(58, 113), S(80, 113), S(99, 123), S(135, 45),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-82, -275), S(-31, -81), S(26, -4), S(53, 45), S(73, 87), S(87, 115), S(94, 140), S(99, 153), S(100, 169), S(111, 160), S(116, 156), S(150, 105), S(161, 97), S(180, 4),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(107, -146), S(18, 60), S(40, 90), S(57, 106), S(68, 124), S(74, 135), S(79, 149), S(85, 155), S(91, 165), S(99, 170), S(107, 173), S(110, 178), S(110, 181), S(115, 159), S(179, 60),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-178, -129), S(-25, 35), S(1, 171), S(22, 295), S(40, 337), S(47, 390), S(55, 420), S(62, 436), S(65, 463), S(69, 477), S(74, 486), S(78, 490), S(80, 498), S(82, 497), S(81, 501), S(79, 502), S(73, 502), S(76, 489), S(76, 484), S(80, 471), S(79, 447), S(96, 414), S(73, 421), S(42, 398), S(28, 380), S(-0, 390), S(-5, 333), S(-14, 290),
};

inline const PParam PAWN_THREAT_KNIGHT = S(212, 136);
inline const PParam PAWN_THREAT_BISHOP = S(190, 197);
inline const PParam PAWN_THREAT_ROOK   = S(224, 119);
inline const PParam PAWN_THREAT_QUEEN  = S(183, 3);

inline const PParam KNIGHT_THREAT_BISHOP = S(114, 121);
inline const PParam KNIGHT_THREAT_ROOK   = S(225, 93);
inline const PParam KNIGHT_THREAT_QUEEN  = S(174, -23);

inline const PParam BISHOP_THREAT_KNIGHT = S(104, 77);
inline const PParam BISHOP_THREAT_ROOK   = S(223, 159);
inline const PParam BISHOP_THREAT_QUEEN  = S(192, 89);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(7, -28), S(-0, -10), S(-4, -19), S(-7, -30), S(-12, -39), S(-16, -47), S(-17, -58), S(-24, -55), S(-26, -75),
};

inline const PParam ROOK_LINEUP = S(17, 80);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(209, 243),    S(69, 324),     S(148, 257),    S(181, 202),    S(203, 141),    S(132, 217),    S(78, 267),     S(219, 203),    //
    S(30, 52),      S(42, 84),      S(51, 9),       S(53, -63),     S(36, -82),     S(-1, -23),     S(-12, 36),     S(-43, 62),     //
    S(26, -15),     S(11, -14),     S(38, -62),     S(28, -92),     S(16, -100),    S(-6, -72),     S(-56, -24),    S(-60, 12),     //
    S(-8, -77),     S(-36, -51),    S(-4, -69),     S(-11, -83),    S(-31, -90),    S(-42, -66),    S(-102, -35),   S(-93, -38),    //
    S(-9, -111),    S(21, -107),    S(7, -45),      S(-9, -56),     S(-36, -63),    S(-54, -60),    S(-91, -45),    S(-95, -54),    //
    S(12, -104),    S(91, -105),    S(74, -51),     S(31, -19),     S(-1, -37),     S(-23, -51),    S(-58, -35),    S(-72, -39),    //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-319, -395),  S(-286, 63),    S(-157, -252),  S(5, 30),       S(-98, 38),     S(-301, 88),    S(-369, 95),    S(-418, -289),  //
    S(-47, 1),      S(-15, 35),     S(69, 27),      S(84, 61),      S(92, 48),      S(40, 22),      S(-39, 32),     S(-54, -1),     //
    S(34, -23),     S(46, 22),      S(73, 69),      S(82, 91),      S(67, 97),      S(16, 95),      S(-3, 45),      S(-58, 26),     //
    S(89, 20),      S(100, 63),     S(107, 86),     S(119, 118),    S(120, 118),    S(71, 98),      S(48, 58),      S(24, 42),      //
    S(74, 13),      S(103, 15),     S(101, 66),     S(86, 96),      S(77, 93),      S(65, 82),      S(53, 27),      S(15, 22),      //
    S(6, -39),      S(32, -3),      S(38, 39),      S(48, 72),      S(44, 67),      S(18, 36),      S(4, 4),        S(-39, -44),    //
    S(25, -32),     S(26, -3),      S(14, -16),     S(19, 15),      S(15, 9),       S(-14, -29),    S(-43, -11),    S(-49, -112),   //
    S(-56, -80),    S(-1, -38),     S(15, -50),     S(30, -32),     S(16, -22),     S(-22, -44),    S(-36, -21),    S(-81, -120),   //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-166, 38),    S(-214, 137),   S(-464, 197),   S(-315, 119),   S(-327, 155),   S(-333, 143),   S(-240, 143),   S(-165, 94),    //
    S(-62, -3),     S(-99, 98),     S(-61, 66),     S(-95, 84),     S(-93, 87),     S(-51, 51),     S(-47, 62),     S(-65, 18),     //
    S(26, 27),      S(12, 62),      S(31, 66),      S(25, 64),      S(18, 50),      S(13, 54),      S(6, 53),       S(5, 6),        //
    S(2, 11),       S(48, 34),      S(61, 54),      S(76, 92),      S(98, 79),      S(46, 47),      S(36, 14),      S(-11, 11),     //
    S(45, -48),     S(52, 3),       S(80, 28),      S(88, 58),      S(77, 62),      S(67, 44),      S(19, 27),      S(12, -28),     //
    S(59, -18),     S(86, -10),     S(97, 19),      S(70, 43),      S(63, 27),      S(54, 35),      S(59, 16),      S(15, -6),      //
    S(33, -71),     S(110, -42),    S(70, -16),     S(44, 9),       S(28, 5),       S(45, -33),     S(42, -41),     S(36, -34),     //
    S(52, -67),     S(32, -20),     S(31, -3),      S(45, -22),     S(31, -10),     S(35, 22),      S(44, -1),      S(39, -40),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(185, 130),    S(238, 111),    S(181, 138),    S(178, 119),    S(227, 87),     S(189, 112),    S(196, 127),    S(177, 139),    //
    S(76, 208),     S(155, 195),    S(233, 147),    S(169, 175),    S(203, 166),    S(183, 172),    S(121, 202),    S(99, 206),     //
    S(56, 185),     S(175, 142),    S(227, 116),    S(207, 93),     S(199, 126),    S(127, 179),    S(129, 174),    S(67, 221),     //
    S(28, 145),     S(111, 154),    S(133, 137),    S(113, 134),    S(142, 119),    S(98, 169),     S(93, 172),     S(20, 197),     //
    S(-3, 77),      S(69, 89),      S(65, 107),     S(25, 124),     S(37, 127),     S(30, 142),     S(11, 142),     S(-10, 143),    //
    S(11, -12),     S(91, 13),      S(67, 37),      S(54, 41),      S(58, 54),      S(37, 73),      S(41, 59),      S(-10, 71),     //
    S(-74, -19),    S(61, -52),     S(61, -17),     S(45, 19),      S(50, 19),      S(36, 29),      S(30, 12),      S(-11, 25),     //
    S(-17, -37),    S(9, -12),      S(68, -20),     S(78, -19),     S(83, -12),     S(58, 11),      S(55, 3),       S(33, 9),       //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(88, 167),     S(160, 138),    S(72, 241),     S(15, 318),     S(54, 285),     S(97, 199),     S(78, 162),     S(36, 201),     //
    S(54, 246),     S(75, 292),     S(63, 304),     S(-89, 424),    S(-40, 412),    S(12, 328),     S(54, 234),     S(30, 221),     //
    S(43, 230),     S(62, 303),     S(24, 369),     S(-7, 379),     S(16, 368),     S(48, 317),     S(78, 232),     S(82, 160),     //
    S(57, 135),     S(48, 241),     S(-1, 322),     S(-9, 380),     S(11, 387),     S(32, 278),     S(68, 204),     S(47, 176),     //
    S(37, 142),     S(29, 171),     S(12, 240),     S(-13, 314),    S(-8, 362),     S(21, 288),     S(20, 228),     S(52, 136),     //
    S(33, 58),      S(52, 84),      S(49, 158),     S(19, 203),     S(27, 224),     S(29, 252),     S(53, 188),     S(39, 138),     //
    S(8, -90),      S(24, -32),     S(27, 30),      S(51, 65),      S(46, 114),     S(46, 99),      S(27, 109),     S(40, 90),      //
    S(-8, -64),     S(16, -203),    S(40, -191),    S(49, -80),     S(53, 7),       S(55, -33),     S(51, -1),      S(20, 52),      //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-21, -261),   S(311, 339),    S(197, 373),    S(-74, 267),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(190, 22),     S(224, 366),    S(102, 348),    S(-67, 249),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(108, 127),    S(151, 253),    S(44, 281),     S(-108, 236),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-27, 56),     S(61, 151),     S(-77, 222),    S(-180, 244),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-91, -26),    S(-41, 65),     S(-134, 151),   S(-254, 222),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-22, -78),    S(36, -12),     S(-86, 91),     S(-166, 157),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(46, -140),    S(51, -62),     S(-28, 1),      S(-104, 65),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-0, -240),    S(28, -135),    S(-61, -80),    S(-59, -90),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const PParam KS_NO_QUEEN = S(-87, -422);

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(8, -3), S(15, -18), S(14, -2), S(6, -5), S(1, -21),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(3, -8), S(7, -20), S(4, -11), S(4, 0), S(4, 5),
};

inline const PParam KS_FLANK_ATTACK = S(4, -8);
inline const PParam KS_FLANK_DEFENSE = S(-5, -0);
inline const PParam KS_FLANK_DOUBLE_ATTACK   = S(3, -5);
inline const PParam KS_FLANK_DOUBLE_DEFENSE  = S(-2, -6);

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(16, 22), S(-5, 22), S(-4, -42), S(6, -28), S(7, -24), S(-10, -3), S(-23, -4), }},
  {{ S(-1, 104), S(-19, -190), S(-19, -39), S(-9, 32), S(-6, 41), S(-20, 67), S(-40, 53), }},
  {{ S(-5, 85), S(-10, -164), S(-10, 82), S(-3, 59), S(-3, 46), S(-12, 43), S(-42, 3), }},
  {{ S(5, 99), S(-12, 68), S(0, 70), S(6, 59), S(4, 40), S(4, 48), S(4, 43), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(15, 106), S(-8, 60), S(-10, 84), S(-3, 101), S(14, 68),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(3, 119), S(-58, -346), S(-10, -263), S(8, -253), S(-6, 55), S(-10, 76), S(-9, 75), }},
  {{ S(8, 47), S(-26, -394), S(2, -380), S(-3, -21), S(-4, 17), S(-11, 33), S(0, 29), }},
  {{ S(0, 69), S(-11, -358), S(7, -32), S(4, 9), S(-2, 37), S(-8, 56), S(-3, 51), }},
  {{ S(-2, 62), S(-4, -146), S(8, -28), S(5, 24), S(-7, 47), S(-14, 51), S(-11, 53), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1094, 308, -15, 57
);

inline VParam WINNABLE_PAWNS = V(-24);
inline VParam WINNABLE_SYM = V(129);
inline VParam WINNABLE_ASYM = V(114);
inline VParam WINNABLE_PAWN_ENDGAME = V(175);
inline VParam WINNABLE_BIAS = V(-504);

// Epoch duration: 4.81574s
// clang-format on
}  // namespace Clockwork
