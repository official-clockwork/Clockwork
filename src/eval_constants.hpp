#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(195, 523);
inline const PParam KNIGHT_MAT = S(813, 1620);
inline const PParam BISHOP_MAT = S(863, 1705);
inline const PParam ROOK_MAT   = S(1035, 2708);
inline const PParam QUEEN_MAT  = S(2411, 4188);

inline const PParam TEMPO_VAL  = S(70, 58);

inline const PParam BISHOP_XRAY_PAWNS   = S(-13, -3);
inline const PParam BISHOP_PAIR_VAL   = S(61, 244);
inline const PParam ROOK_OPEN_VAL     = S(112, -6);
inline const PParam ROOK_SEMIOPEN_VAL = S(47, 9);
inline const PParam MINOR_BEHIND_PAWN = S(16, 40);
inline const PParam RESTRICTED_SQUARES = S(19, 7);
inline const PParam SPACE_VAL = S(80, -72);

inline const PParam DOUBLED_PAWN_VAL = S(-15, -89);
inline const PParam ISOLATED_PAWN_VAL = S(-13, -40);

inline const PParam POTENTIAL_CHECKER_VAL = S(-45, -43);
inline const PParam OUTPOST_KNIGHT_VAL    = S(50, 43);
inline const PParam OUTPOST_BISHOP_VAL    = S(42, 38);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(37, 55);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(40, 7);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(18, 71);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(60, -44);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(19, -7), S(35, 34), S(51, 76), S(120, 217), S(397, 403), S(683, 824),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(53, 28), S(40, 25), S(42, 68), S(88, 183), S(395, 118),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-93, -262), S(-94, -220), S(-68, -85), S(-20, 40), S(87, 206), S(225, 421),
};
inline const std::array<PParam, 6> PASSED_CLEAR_STOPPERS = {
    S(-78, 50), S(-92, 35), S(-56, 104), S(-91, 234), S(-52, 342), S(52, 226),
};
inline const std::array<PParam, 6> PASSED_CLEAR_FORWARD = {
    S(-17, 69), S(-28, 59), S(-57, 106), S(-28, 132), S(78, 156), S(34, 197),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(22, -38), S(23, -0), S(19, 33), S(25, 101), S(74, 273), S(336, 224),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(11, -34), S(-0, 11), S(-5, -25), S(-3, -66), S(-16, -177), S(-170, -463),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(24, 232), S(13, 180), S(7, 103), S(14, 65), S(22, 70), S(60, 65), S(65, 34),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-125, -60), S(18, -10), S(5, 96), S(30, 149), S(44, 186), S(54, 195), S(40, 176),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-87, -411), S(-21, -151), S(6, -29), S(27, 26), S(52, 63), S(67, 102), S(85, 109), S(106, 116), S(126, 68),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-68, -360), S(-15, -135), S(38, -43), S(60, 17), S(77, 64), S(87, 98), S(92, 121), S(95, 142), S(99, 154), S(105, 162), S(114, 154), S(128, 138), S(127, 142), S(113, 106),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(101, -212), S(22, -6), S(44, 27), S(60, 44), S(71, 63), S(74, 79), S(78, 95), S(84, 102), S(88, 115), S(93, 123), S(97, 129), S(97, 139), S(100, 144), S(106, 130), S(125, 92),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-195, -203), S(-37, -91), S(-11, 47), S(8, 188), S(26, 232), S(33, 287), S(38, 328), S(43, 350), S(45, 379), S(47, 400), S(52, 411), S(56, 419), S(60, 425), S(63, 430), S(65, 430), S(65, 433), S(62, 434), S(65, 426), S(66, 422), S(70, 414), S(59, 406), S(71, 377), S(36, 407), S(-10, 399), S(-42, 395), S(-77, 417), S(-138, 447), S(-102, 381),
};

inline const PParam PAWN_THREAT_KNIGHT = S(197, 159);
inline const PParam PAWN_THREAT_BISHOP = S(169, 228);
inline const PParam PAWN_THREAT_ROOK   = S(191, 156);
inline const PParam PAWN_THREAT_QUEEN  = S(156, 33);

inline const std::array<std::array<PParam, 5>, 2> MINOR_THREAT = {{
  {{ S(9, 57), S(98, 98), S(104, 127), S(179, 95), S(165, 13), }},
  {{ S(7, 57), S(87, 96), S(103, 126), S(220, 620), S(149, 53), }},
}};
inline const std::array<std::array<PParam, 5>, 2> ROOK_THREAT = {{
  {{ S(10, 66), S(44, 110), S(58, 114), S(13, 11), S(152, -77), }},
  {{ S(3, 51), S(61, 84), S(95, 73), S(23, 13), S(368, 895), }},
}};
inline const PParam KING_THREAT  = S(-11, 159);
inline const PParam HANGING_PAWN  = S(32, 93);
inline const PParam HANGING_NON_PAWN  = S(73, 31);

inline const std::array<PParam, 2> KNIGHT_ON_QUEEN = {
    S(21, -15), S(22, 9),
};
inline const std::array<PParam, 2> BISHOP_ON_QUEEN = {
    S(44, 43), S(113, -112),
};
inline const std::array<PParam, 2> ROOK_ON_QUEEN = {
    S(39, 10), S(50, -8),
};

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(5, -19), S(5, -16), S(0, -22), S(-6, -30), S(-12, -37), S(-18, -42), S(-21, -53), S(-27, -55), S(-36, -69),
};

inline const PParam ROOK_LINEUP = S(17, 81);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(186, 279),    S(59, 407),     S(63, 398),     S(154, 277),    S(189, 173),    S(142, 215),    S(121, 247),    S(224, 182),    //
    S(41, 66),      S(42, 121),     S(15, 75),      S(23, 13),      S(14, -23),     S(-6, 0),       S(2, 37),       S(-20, 54),     //
    S(38, -13),     S(9, -2),       S(24, -32),     S(-4, -46),     S(-14, -63),    S(-16, -61),    S(-41, -42),    S(-38, -6),     //
    S(-8, -74),     S(-35, -42),    S(-8, -57),     S(-25, -66),    S(-47, -75),    S(-44, -71),    S(-86, -58),    S(-78, -54),    //
    S(-20, -108),   S(12, -103),    S(-0, -42),     S(-15, -50),    S(-40, -62),    S(-56, -70),    S(-86, -69),    S(-90, -72),    //
    S(2, -103),     S(76, -89),     S(78, -45),     S(31, -17),     S(2, -40),      S(-20, -65),    S(-58, -52),    S(-70, -53),    //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-267, -496),  S(-263, 57),    S(-269, -26),   S(-0, 44),      S(-91, 43),     S(-269, 85),    S(-351, 106),   S(-356, -400),  //
    S(-1, -10),     S(3, 30),       S(88, 22),      S(85, 61),      S(91, 48),      S(51, 29),      S(-16, 27),     S(-44, 24),     //
    S(41, -23),     S(43, 29),      S(57, 63),      S(77, 70),      S(60, 72),      S(23, 56),      S(11, 33),      S(-36, 13),     //
    S(89, 27),      S(90, 50),      S(100, 72),     S(101, 115),    S(106, 113),    S(71, 75),      S(50, 42),      S(32, 35),      //
    S(79, 17),      S(104, 1),      S(102, 48),     S(95, 82),      S(82, 85),      S(74, 64),      S(61, 16),      S(24, 27),      //
    S(20, -42),     S(47, -29),     S(52, 19),      S(63, 53),      S(58, 52),      S(31, 15),      S(18, -22),     S(-22, -42),    //
    S(40, -27),     S(41, -13),     S(30, -21),     S(39, 7),       S(36, 4),       S(7, -46),      S(-28, -2),     S(-49, -82),    //
    S(-49, -82),    S(9, -27),      S(28, -41),     S(42, -28),     S(26, -15),     S(-12, -47),    S(-24, -22),    S(-77, -130),   //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-131, 23),    S(-198, 111),   S(-406, 173),   S(-277, 103),   S(-308, 144),   S(-308, 157),   S(-233, 134),   S(-130, 75),    //
    S(-26, -20),    S(-65, 91),     S(-45, 62),     S(-68, 61),     S(-67, 77),     S(-39, 49),     S(-9, 43),      S(-52, 27),     //
    S(37, 16),      S(15, 61),      S(31, 69),      S(25, 57),      S(21, 44),      S(22, 47),      S(8, 46),       S(13, 13),      //
    S(15, -3),      S(47, 24),      S(59, 39),      S(77, 59),      S(100, 46),     S(41, 21),      S(42, -0),      S(-3, 1),       //
    S(35, -43),     S(43, 3),       S(75, 16),      S(91, 30),      S(74, 41),      S(64, 28),      S(18, 8),       S(7, -35),      //
    S(55, -29),     S(81, -25),     S(96, 6),       S(62, 32),      S(58, 18),      S(57, 15),      S(59, -3),      S(15, -22),     //
    S(26, -73),     S(116, -62),    S(67, -20),     S(49, -4),      S(32, -4),      S(45, -44),     S(47, -61),     S(37, -49),     //
    S(40, -67),     S(21, -21),     S(27, -5),      S(41, -28),     S(25, -16),     S(34, 19),      S(40, -10),     S(35, -54),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(172, 111),    S(199, 134),    S(151, 165),    S(136, 137),    S(198, 91),     S(155, 119),    S(165, 133),    S(147, 132),    //
    S(82, 155),     S(139, 160),    S(183, 131),    S(137, 116),    S(174, 103),    S(141, 128),    S(92, 162),     S(84, 162),     //
    S(52, 149),     S(173, 92),     S(200, 75),     S(171, 60),     S(175, 77),     S(117, 119),    S(109, 138),    S(57, 175),     //
    S(26, 115),     S(89, 123),     S(109, 98),     S(92, 98),      S(120, 81),     S(80, 122),     S(71, 136),     S(17, 161),     //
    S(2, 45),       S(61, 55),      S(53, 79),      S(23, 87),      S(33, 92),      S(22, 116),     S(4, 114),      S(-16, 117),    //
    S(15, -36),     S(82, -9),      S(69, 18),      S(47, 30),      S(57, 36),      S(34, 60),      S(32, 42),      S(-7, 49),      //
    S(-66, -41),    S(59, -76),     S(64, -35),     S(53, -4),      S(57, -4),      S(42, 10),      S(33, -7),      S(3, -5),       //
    S(-2, -63),     S(21, -31),     S(84, -42),     S(93, -42),     S(95, -37),     S(71, -14),     S(67, -22),     S(46, -17),     //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(87, 153),     S(140, 159),    S(47, 308),     S(-11, 406),    S(27, 357),     S(62, 266),     S(66, 178),     S(23, 207),     //
    S(54, 188),     S(70, 248),     S(30, 329),     S(-88, 443),    S(-46, 412),    S(12, 295),     S(52, 179),     S(36, 149),     //
    S(19, 234),     S(54, 266),     S(23, 343),     S(-19, 386),    S(1, 371),      S(46, 262),     S(77, 158),     S(60, 123),     //
    S(13, 171),     S(23, 242),     S(-20, 313),    S(-18, 364),    S(-2, 359),     S(15, 243),     S(55, 159),     S(31, 119),     //
    S(10, 144),     S(7, 168),      S(-1, 219),     S(-25, 286),    S(-13, 302),    S(7, 242),      S(13, 174),     S(34, 80),      //
    S(2, 49),       S(34, 56),      S(30, 133),     S(8, 157),      S(17, 166),     S(20, 188),     S(41, 112),     S(20, 93),      //
    S(-24, -104),   S(7, -105),     S(15, -34),     S(42, 5),       S(39, 45),      S(41, 18),      S(15, 53),      S(27, 25),      //
    S(-43, -86),    S(-11, -246),   S(20, -251),    S(34, -142),    S(42, -67),     S(40, -85),     S(36, -66),     S(3, -10),      //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(169, -558),   S(481, 85),     S(319, 197),    S(23, 191),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(374, -186),   S(368, 193),    S(216, 204),    S(32, 153),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(203, 44),     S(235, 178),    S(118, 200),    S(-46, 178),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(44, 6),       S(119, 113),    S(-18, 171),    S(-113, 200),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-107, -5),    S(-17, 75),     S(-80, 143),    S(-176, 194),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-51, -37),    S(17, 31),      S(-63, 112),    S(-121, 157),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(24, -93),     S(29, -10),     S(-34, 52),     S(-113, 112),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-22, -214),   S(2, -91),      S(-78, -24),    S(-88, -36),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const PParam KS_NO_QUEEN = S(-81, -447);

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(7, -3), S(10, 12), S(10, 5), S(4, 1), S(3, -10),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(4, -6), S(5, 4), S(2, 2), S(3, -1), S(4, -2),
};

inline const PParam KS_FLANK_ATTACK = S(3, -2);
inline const PParam KS_FLANK_DEFENSE = S(-3, -0);
inline const PParam KS_FLANK_DOUBLE_ATTACK   = S(3, -1);
inline const PParam KS_FLANK_DOUBLE_DEFENSE  = S(-2, 2);

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(16, -21), S(-7, 3), S(-3, -12), S(6, -18), S(11, -30), S(6, -55), S(-4, -49), }},
  {{ S(2, 2), S(-21, 0), S(-17, 8), S(-10, 16), S(-7, 7), S(-10, -14), S(-20, -23), }},
  {{ S(-4, -1), S(-12, -8), S(-13, 18), S(-9, 19), S(-8, 10), S(-7, -17), S(-23, -39), }},
  {{ S(5, 10), S(-11, 20), S(-8, 41), S(-3, 45), S(-3, 38), S(3, 20), S(8, -12), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(10, 23), S(-6, 3), S(-7, 9), S(-5, 27), S(2, 48),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(5, 11), S(-42, -127), S(-9, -42), S(-1, 4), S(-3, 4), S(-6, 11), S(-5, 10), }},
  {{ S(9, -1), S(-25, -132), S(-7, -30), S(-4, 0), S(-2, -2), S(-9, 5), S(1, 4), }},
  {{ S(0, 10), S(-7, -76), S(7, -16), S(0, 8), S(-4, 11), S(-9, 18), S(-6, 20), }},
  {{ S(1, -2), S(3, -71), S(-0, 22), S(-2, 25), S(-6, 8), S(-12, 8), S(-8, 16), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
	1421, 897, -21, 1
);

inline VParam WINNABLE_PAWNS = V(-20);
inline VParam WINNABLE_SYM = V(104);
inline VParam WINNABLE_ASYM = V(89);
inline VParam WINNABLE_PAWN_ENDGAME = V(73);
inline VParam WINNABLE_BIAS = V(-403);

// Epoch duration: 9.56388s
// Epoch duration: 7.65336s
// clang-format on
}  // namespace Clockwork
