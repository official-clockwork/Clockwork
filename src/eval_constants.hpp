#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(188, 452);
inline const PParam KNIGHT_MAT = S(851, 1362);
inline const PParam BISHOP_MAT = S(892, 1443);
inline const PParam ROOK_MAT   = S(1069, 2334);
inline const PParam QUEEN_MAT  = S(2581, 3690);
inline const PParam TEMPO_VAL  = S(63, 22);

inline const PParam BISHOP_XRAY_PAWNS   = S(-14, 15);
inline const PParam BISHOP_PAIR_VAL   = S(56, 227);
inline const PParam ROOK_OPEN_VAL     = S(122, -13);
inline const PParam ROOK_SEMIOPEN_VAL = S(46, 32);
inline const PParam MINOR_BEHIND_PAWN = S(15, 43);
inline const PParam RESTRICTED_SQUARES = S(-24, -7);

inline const PParam DOUBLED_PAWN_VAL = S(-15, -80);
inline const PParam ISOLATED_PAWN_VAL = S(-16, -26);

inline const PParam POTENTIAL_CHECKER_VAL = S(-50, -25);
inline const PParam OUTPOST_KNIGHT_VAL    = S(56, 58);
inline const PParam OUTPOST_BISHOP_VAL    = S(57, 42);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(38, 6);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(45, -16);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(24, 56);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(61, -38);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(17, 0), S(32, 33), S(61, 62), S(130, 178), S(438, 239), S(569, 607),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(46, 41), S(44, 37), S(64, 65), S(123, 165), S(466, 83),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-96, -237), S(-97, -203), S(-73, -76), S(-24, 43), S(82, 213), S(243, 324),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(28, -42), S(21, 1), S(13, 38), S(9, 118), S(84, 230), S(172, 392),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(14, -24), S(3, 20), S(-5, -17), S(-2, -47), S(-16, -138), S(-213, -266),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(15, 219), S(-11, 186), S(-7, 105), S(7, 60), S(20, 59), S(61, 54), S(48, 41),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-221, -51), S(-5, 38), S(1, 106), S(38, 134), S(52, 164), S(63, 171), S(39, 167),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-99, -322), S(-35, -124), S(-3, -18), S(23, 19), S(54, 40), S(69, 74), S(90, 74), S(109, 83), S(143, 8),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-66, -275), S(-16, -119), S(35, -54), S(57, -3), S(79, 36), S(92, 64), S(98, 85), S(105, 98), S(105, 115), S(117, 105), S(121, 102), S(158, 50), S(162, 46), S(181, -26),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(115, -189), S(23, -11), S(45, 19), S(61, 32), S(72, 48), S(76, 60), S(82, 73), S(88, 77), S(93, 88), S(101, 92), S(109, 95), S(112, 101), S(112, 103), S(119, 80), S(191, -30),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-189, -133), S(-42, -35), S(-15, 86), S(7, 199), S(24, 236), S(31, 282), S(38, 314), S(44, 327), S(47, 351), S(50, 367), S(55, 374), S(58, 383), S(62, 384), S(63, 387), S(63, 388), S(60, 390), S(55, 389), S(59, 375), S(60, 367), S(65, 353), S(65, 331), S(77, 295), S(56, 307), S(29, 275), S(6, 264), S(-17, 255), S(-58, 257), S(-58, 211),
};

inline const PParam PAWN_THREAT_KNIGHT = S(208, 121);
inline const PParam PAWN_THREAT_BISHOP = S(191, 178);
inline const PParam PAWN_THREAT_ROOK   = S(210, 130);
inline const PParam PAWN_THREAT_QUEEN  = S(177, 3);

inline const PParam KNIGHT_THREAT_BISHOP = S(109, 118);
inline const PParam KNIGHT_THREAT_ROOK   = S(225, 88);
inline const PParam KNIGHT_THREAT_QUEEN  = S(159, -4);

inline const PParam BISHOP_THREAT_KNIGHT = S(98, 75);
inline const PParam BISHOP_THREAT_ROOK   = S(212, 152);
inline const PParam BISHOP_THREAT_QUEEN  = S(178, 101);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(8, -28), S(0, -9), S(-2, -19), S(-6, -30), S(-11, -40), S(-15, -47), S(-16, -60), S(-21, -62), S(-28, -72),
};

inline const PParam ROOK_LINEUP = S(17, 73);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(207, 223),    S(54, 312),     S(129, 259),    S(157, 203),    S(197, 130),    S(111, 215),    S(75, 255),     S(222, 194),    //
    S(31, 47),      S(37, 83),      S(33, 13),      S(43, -57),     S(32, -85),     S(-4, -26),     S(-17, 35),     S(-36, 61),     //
    S(25, -20),     S(5, -15),      S(32, -62),     S(23, -89),     S(14, -102),    S(-11, -72),    S(-52, -27),    S(-54, 8),      //
    S(-10, -78),    S(-33, -52),    S(-1, -73),     S(-8, -82),     S(-29, -88),    S(-41, -67),    S(-96, -34),    S(-87, -40),    //
    S(-9, -111),    S(25, -110),    S(11, -49),     S(-4, -56),     S(-31, -66),    S(-50, -62),    S(-84, -50),    S(-87, -56),    //
    S(12, -107),    S(90, -104),    S(70, -52),     S(29, -18),     S(1, -43),      S(-22, -54),    S(-52, -39),    S(-66, -41),    //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-284, -449),  S(-267, 29),    S(-297, -85),   S(17, 21),      S(-88, 35),     S(-258, 49),    S(-396, 100),   S(-375, -330),  //
    S(-30, -18),    S(-21, 27),     S(85, 1),       S(84, 40),      S(97, 27),      S(36, 20),      S(-32, 20),     S(-43, -7),     //
    S(41, -35),     S(39, 14),      S(79, 54),      S(86, 76),      S(71, 85),      S(19, 82),      S(6, 20),       S(-41, 6),      //
    S(100, 12),     S(101, 46),     S(109, 70),     S(113, 109),    S(118, 106),    S(77, 85),      S(54, 43),      S(38, 24),      //
    S(75, 4),       S(113, -5),     S(103, 55),     S(90, 81),      S(78, 79),      S(76, 69),      S(65, 8),       S(21, 17),      //
    S(17, -51),     S(44, -17),     S(43, 26),      S(57, 55),      S(50, 53),      S(24, 24),      S(14, -6),      S(-28, -47),    //
    S(35, -37),     S(39, -25),     S(20, -25),     S(25, 3),       S(22, 0),       S(-1, -50),     S(-30, -19),    S(-37, -121),   //
    S(-38, -99),    S(10, -42),     S(30, -65),     S(47, -54),     S(29, -36),     S(-9, -66),     S(-20, -39),    S(-61, -145),   //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-132, 22),    S(-180, 96),    S(-488, 189),   S(-305, 103),   S(-314, 124),   S(-317, 118),   S(-208, 100),   S(-133, 50),    //
    S(-51, -15),    S(-96, 82),     S(-47, 33),     S(-100, 71),    S(-86, 60),     S(-46, 30),     S(-26, 26),     S(-50, -3),     //
    S(34, 12),      S(19, 42),      S(31, 52),      S(24, 49),      S(15, 34),      S(11, 36),      S(15, 28),      S(17, -7),      //
    S(11, -8),      S(53, 9),       S(70, 28),      S(77, 65),      S(104, 53),     S(49, 22),      S(48, -9),      S(-0, -9),      //
    S(52, -62),     S(54, -13),     S(83, 7),       S(96, 34),      S(83, 44),      S(71, 25),      S(23, 4),       S(21, -40),     //
    S(66, -39),     S(90, -31),     S(103, -1),     S(73, 24),      S(70, 6),       S(67, 14),      S(75, -7),      S(23, -23),     //
    S(41, -96),     S(118, -64),    S(77, -36),     S(52, -13),     S(37, -12),     S(56, -52),     S(54, -57),     S(49, -67),     //
    S(59, -93),     S(35, -44),     S(31, -14),     S(53, -41),     S(38, -33),     S(42, 4),       S(53, -30),     S(49, -63),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(192, 85),     S(232, 84),     S(182, 105),    S(171, 80),     S(228, 55),     S(179, 83),     S(204, 90),     S(183, 100),    //
    S(91, 160),     S(156, 155),    S(235, 106),    S(168, 131),    S(213, 119),    S(174, 135),    S(112, 164),    S(101, 166),    //
    S(55, 153),     S(186, 106),    S(220, 82),     S(199, 66),     S(193, 92),     S(130, 135),    S(134, 135),    S(70, 176),     //
    S(31, 110),     S(108, 120),    S(136, 94),     S(108, 97),     S(143, 82),     S(99, 128),     S(96, 130),     S(26, 153),     //
    S(2, 35),       S(78, 48),      S(66, 66),      S(34, 69),      S(46, 78),      S(36, 105),     S(15, 100),     S(-5, 96),      //
    S(16, -50),     S(95, -29),     S(77, -5),      S(56, -4),      S(66, 5),       S(38, 39),      S(47, 16),      S(-1, 25),      //
    S(-74, -51),    S(67, -91),     S(65, -55),     S(50, -23),     S(56, -23),     S(41, -10),     S(34, -29),     S(2, -19),      //
    S(-12, -77),    S(5, -44),      S(70, -56),     S(80, -58),     S(80, -46),     S(59, -24),     S(58, -36),     S(40, -30),     //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(49, 141),     S(155, 66),     S(64, 181),     S(9, 265),      S(48, 225),     S(72, 168),     S(67, 111),     S(-9, 175),     //
    S(49, 181),     S(54, 261),     S(33, 275),     S(-105, 371),   S(-58, 366),    S(-6, 288),     S(34, 184),     S(14, 176),     //
    S(20, 196),     S(45, 267),     S(10, 323),     S(-26, 339),    S(-11, 338),    S(34, 264),     S(65, 179),     S(62, 102),     //
    S(39, 97),      S(29, 199),     S(-15, 273),    S(-36, 344),    S(-18, 360),    S(19, 232),     S(62, 150),     S(36, 127),     //
    S(26, 89),      S(16, 132),     S(-3, 195),     S(-28, 269),    S(-16, 304),    S(8, 239),      S(20, 160),     S(45, 79),      //
    S(27, 2),       S(42, 35),      S(36, 121),     S(12, 141),     S(19, 164),     S(21, 200),     S(42, 136),     S(36, 75),      //
    S(-5, -128),    S(18, -78),     S(20, -18),     S(44, 17),      S(37, 65),      S(39, 45),      S(15, 71),      S(34, 34),      //
    S(-16, -118),   S(4, -260),     S(26, -238),    S(39, -127),    S(41, -28),     S(47, -80),     S(44, -52),     S(18, -14),     //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(37, -296),    S(322, 305),    S(203, 331),    S(-23, 225),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(246, 0),      S(270, 277),    S(118, 315),    S(-13, 209),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(143, 121),    S(167, 239),    S(62, 276),     S(-64, 225),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-29, 63),     S(61, 158),     S(-80, 233),    S(-144, 245),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-101, -16),   S(-40, 75),     S(-129, 156),   S(-236, 224),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-20, -68),    S(31, 0),       S(-87, 97),     S(-158, 158),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(41, -126),    S(47, -49),     S(-34, 13),     S(-111, 72),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-4, -225),    S(20, -119),    S(-73, -62),    S(-58, -86),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const PParam KS_NO_QUEEN = S(-84, -541);

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(8, -7), S(14, -24), S(13, -3), S(6, -8), S(1, -25),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(3, -11), S(6, -24), S(4, -13), S(4, -0), S(4, 4),
};

inline const PParam KS_FLANK_ATTACK = S(4, -8);
inline const PParam KS_FLANK_DEFENSE = S(-4, 1);
inline const PParam KS_FLANK_DOUBLE_ATTACK   = S(3, -5);
inline const PParam KS_FLANK_DOUBLE_DEFENSE  = S(-2, -8);

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(15, 27), S(-6, 24), S(-5, -55), S(6, -35), S(7, -23), S(-11, 11), S(-23, 9), }},
  {{ S(-2, 129), S(-20, -264), S(-20, -52), S(-9, 34), S(-8, 61), S(-21, 95), S(-39, 91), }},
  {{ S(-4, 111), S(-9, -228), S(-9, 105), S(-3, 77), S(-3, 69), S(-13, 73), S(-40, 55), }},
  {{ S(4, 119), S(-11, 81), S(-2, 90), S(4, 73), S(2, 60), S(5, 73), S(-0, 71), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(14, 121), S(-8, 79), S(-10, 97), S(-3, 110), S(13, 73),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(3, 145), S(-53, -443), S(-11, -357), S(8, -382), S(-5, 64), S(-9, 87), S(-8, 89), }},
  {{ S(8, 55), S(-27, -515), S(-2, -364), S(-3, -32), S(-3, 13), S(-10, 38), S(0, 35), }},
  {{ S(-1, 81), S(-10, -445), S(4, -4), S(2, 9), S(-4, 41), S(-9, 62), S(-4, 56), }},
  {{ S(-1, 72), S(-3, -141), S(6, -8), S(4, 31), S(-6, 49), S(-13, 59), S(-10, 61), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1150, 290, -14, 71
);

inline VParam WINNABLE_PAWNS = V(-24);
inline VParam WINNABLE_SYM = V(131);
inline VParam WINNABLE_ASYM = V(113);
inline VParam WINNABLE_PAWN_ENDGAME = V(186);
inline VParam WINNABLE_BIAS = V(-508);

// Epoch duration: 7.10261s
// clang-format on
}  // namespace Clockwork
