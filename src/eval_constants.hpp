#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(192, 495);
inline const PParam KNIGHT_MAT = S(808, 1534);
inline const PParam BISHOP_MAT = S(852, 1627);
inline const PParam ROOK_MAT   = S(1013, 2567);
inline const PParam QUEEN_MAT  = S(2395, 3888);
inline const PParam TEMPO_VAL  = S(68, 48);

inline const PParam BISHOP_XRAY_PAWNS   = S(-13, -5);
inline const PParam BISHOP_PAIR_VAL   = S(63, 224);
inline const PParam ROOK_OPEN_VAL     = S(109, -5);
inline const PParam ROOK_SEMIOPEN_VAL = S(44, 12);
inline const PParam MINOR_BEHIND_PAWN = S(15, 37);
inline const PParam RESTRICTED_SQUARES = S(-23, -6);

inline const PParam DOUBLED_PAWN_VAL = S(-20, -79);
inline const PParam ISOLATED_PAWN_VAL = S(-13, -36);

inline const PParam POTENTIAL_CHECKER_VAL = S(-46, -39);
inline const PParam OUTPOST_KNIGHT_VAL    = S(53, 41);
inline const PParam OUTPOST_BISHOP_VAL    = S(44, 36);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(37, 52);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(40, 3);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(25, 53);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(60, -48);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(14, -3), S(31, 35), S(59, 64), S(127, 194), S(417, 289), S(582, 606),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(44, 34), S(37, 24), S(54, 52), S(102, 165), S(405, 90),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-88, -265), S(-87, -230), S(-65, -93), S(-24, 41), S(70, 231), S(214, 400),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(26, -43), S(25, -3), S(14, 38), S(12, 115), S(76, 244), S(113, 438),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(14, -39), S(2, 10), S(-7, -22), S(-4, -58), S(-8, -166), S(-168, -330),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(13, 239), S(-0, 189), S(-1, 113), S(9, 74), S(20, 75), S(60, 72), S(50, 53),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-140, -50), S(15, 4), S(1, 106), S(26, 155), S(37, 191), S(50, 196), S(34, 179),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-85, -367), S(-25, -139), S(5, -22), S(28, 21), S(57, 48), S(72, 81), S(90, 86), S(109, 92), S(127, 45),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-61, -278), S(-14, -113), S(35, -43), S(57, 6), S(79, 44), S(91, 70), S(98, 91), S(103, 108), S(107, 120), S(115, 125), S(125, 116), S(142, 100), S(147, 104), S(125, 78),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(104, -191), S(24, -13), S(44, 21), S(60, 35), S(71, 53), S(74, 67), S(79, 81), S(85, 86), S(90, 98), S(97, 103), S(103, 108), S(106, 113), S(109, 118), S(113, 107), S(143, 56),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-205, -132), S(-59, -21), S(-33, 101), S(-12, 223), S(3, 263), S(10, 313), S(17, 345), S(22, 361), S(26, 385), S(28, 403), S(34, 411), S(36, 421), S(39, 426), S(40, 432), S(40, 435), S(36, 441), S(33, 440), S(34, 434), S(34, 431), S(39, 422), S(35, 407), S(38, 385), S(6, 415), S(-34, 396), S(-64, 397), S(-99, 419), S(-145, 443), S(-116, 357),
};

inline const PParam PAWN_THREAT_KNIGHT = S(193, 141);
inline const PParam PAWN_THREAT_BISHOP = S(171, 205);
inline const PParam PAWN_THREAT_ROOK   = S(192, 141);
inline const PParam PAWN_THREAT_QUEEN  = S(148, 12);

inline const std::array<PParam, 6> MINOR_THREAT = {
    S(7, 51), S(92, 90), S(104, 119), S(202, 117), S(160, 12), S(0, 0),
};
inline const std::array<PParam, 6> ROOK_THREAT = {
    S(7, 45), S(49, 85), S(68, 90), S(-25, 9), S(177, -55), S(0, 0),
};
inline const PParam KING_THREAT  = S(-27, 158);
inline const PParam HANGING_PAWN  = S(30, 93);
inline const PParam HANGING_NON_PAWN  = S(67, 25);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(5, -17), S(4, -14), S(-0, -20), S(-5, -28), S(-11, -37), S(-16, -43), S(-18, -56), S(-23, -60), S(-27, -77),
};

inline const PParam ROOK_LINEUP = S(15, 77);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(183, 270),    S(43, 381),     S(65, 354),     S(134, 255),    S(190, 161),    S(122, 221),    S(88, 265),     S(227, 201),    //
    S(36, 68),      S(46, 110),     S(28, 51),      S(42, -23),     S(36, -60),     S(7, -18),      S(2, 38),       S(-22, 68),     //
    S(33, -8),      S(6, 1),        S(38, -46),     S(24, -69),     S(15, -87),     S(-5, -71),     S(-40, -34),    S(-41, 4),      //
    S(-9, -69),     S(-33, -38),    S(1, -63),      S(-9, -74),     S(-30, -84),    S(-39, -72),    S(-87, -48),    S(-78, -47),    //
    S(-12, -106),   S(21, -105),    S(7, -48),      S(-9, -53),     S(-36, -64),    S(-52, -71),    S(-80, -67),    S(-81, -70),    //
    S(10, -104),    S(83, -94),     S(70, -44),     S(25, -18),     S(-5, -42),     S(-24, -62),    S(-50, -55),    S(-61, -53),    //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-251, -508),  S(-242, 37),    S(-262, -91),   S(27, 15),      S(-79, 30),     S(-241, 53),    S(-358, 99),    S(-335, -389),  //
    S(-4, -23),     S(-2, 23),      S(101, 8),      S(82, 51),      S(93, 38),      S(53, 17),      S(-15, 15),     S(-28, 4),      //
    S(44, -33),     S(36, 28),      S(75, 48),      S(79, 64),      S(66, 64),      S(28, 54),      S(13, 26),      S(-35, 4),      //
    S(98, 15),      S(91, 39),      S(102, 65),     S(104, 106),    S(111, 106),    S(76, 70),      S(51, 34),      S(40, 24),      //
    S(76, 12),      S(104, 2),      S(101, 53),     S(94, 83),      S(81, 83),      S(79, 63),      S(65, 7),       S(25, 22),      //
    S(17, -38),     S(45, -18),     S(48, 26),      S(63, 58),      S(58, 57),      S(30, 19),      S(17, -9),      S(-24, -42),    //
    S(39, -26),     S(44, -20),     S(27, -12),     S(34, 16),      S(31, 10),      S(5, -41),      S(-27, -6),     S(-36, -103),   //
    S(-41, -84),    S(8, -26),      S(28, -41),     S(47, -33),     S(27, -12),     S(-10, -51),    S(-18, -30),    S(-60, -141),   //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-116, 19),    S(-165, 93),    S(-428, 185),   S(-263, 85),    S(-283, 131),   S(-287, 139),   S(-191, 105),   S(-114, 55),    //
    S(-28, -16),    S(-75, 91),     S(-34, 50),     S(-70, 67),     S(-62, 66),     S(-27, 38),     S(-3, 31),      S(-42, 18),     //
    S(39, 23),      S(12, 66),      S(31, 73),      S(28, 62),      S(23, 41),      S(20, 46),      S(13, 48),      S(21, 14),      //
    S(16, -4),      S(57, 17),      S(66, 37),      S(77, 56),      S(104, 42),     S(48, 23),      S(54, -7),      S(7, -6),       //
    S(52, -54),     S(50, -2),      S(81, 11),      S(99, 25),      S(80, 42),      S(71, 20),      S(23, 7),       S(23, -35),     //
    S(63, -37),     S(85, -27),     S(100, 5),      S(69, 34),      S(67, 13),      S(64, 16),      S(71, -11),     S(23, -22),     //
    S(35, -91),     S(116, -54),    S(75, -16),     S(52, -0),      S(35, 3),       S(55, -43),     S(53, -52),     S(48, -64),     //
    S(52, -83),     S(23, -19),     S(25, -2),      S(50, -31),     S(33, -22),     S(39, 12),      S(48, -23),     S(45, -62),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(174, 82),     S(212, 99),     S(155, 133),    S(145, 107),    S(204, 64),     S(163, 84),     S(184, 87),     S(156, 94),     //
    S(90, 138),     S(149, 133),    S(197, 102),    S(145, 102),    S(191, 80),     S(142, 109),    S(98, 135),     S(93, 138),     //
    S(64, 131),     S(184, 81),     S(209, 62),     S(183, 47),     S(182, 66),     S(130, 98),     S(131, 107),    S(75, 152),     //
    S(39, 98),      S(105, 104),    S(127, 79),     S(106, 79),     S(135, 66),     S(98, 105),     S(93, 113),     S(32, 141),     //
    S(10, 36),      S(80, 46),      S(68, 62),      S(42, 68),      S(53, 74),      S(39, 102),     S(18, 101),     S(-4, 102),     //
    S(22, -34),     S(93, -11),     S(82, 12),      S(63, 16),      S(73, 22),      S(41, 57),      S(47, 34),      S(1, 44),       //
    S(-66, -27),    S(69, -72),     S(72, -33),     S(61, -1),      S(64, -4),      S(48, 12),      S(36, -3),      S(9, 3),        //
    S(-3, -58),     S(11, -14),     S(83, -32),     S(93, -36),     S(92, -28),     S(69, -4),      S(68, -15),     S(49, -10),     //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(59, 165),     S(140, 123),    S(45, 271),     S(-9, 366),     S(31, 318),     S(63, 231),     S(61, 149),     S(-7, 205),     //
    S(51, 178),     S(50, 244),     S(15, 313),     S(-122, 438),   S(-70, 406),    S(-18, 303),    S(30, 163),     S(15, 159),     //
    S(-1, 247),     S(30, 283),     S(-4, 355),     S(-48, 399),    S(-28, 382),    S(25, 261),     S(58, 155),     S(50, 101),     //
    S(18, 154),     S(4, 256),      S(-26, 305),    S(-50, 385),    S(-31, 380),    S(5, 240),      S(46, 150),     S(23, 122),     //
    S(7, 147),      S(0, 180),      S(-15, 234),    S(-38, 299),    S(-26, 319),    S(-2, 243),     S(8, 165),      S(34, 71),      //
    S(8, 50),       S(29, 72),      S(23, 152),     S(1, 166),      S(12, 169),     S(13, 200),     S(33, 123),     S(23, 84),      //
    S(-19, -101),   S(10, -91),     S(13, -12),     S(37, 23),      S(30, 65),      S(35, 37),      S(8, 74),       S(22, 57),      //
    S(-33, -77),    S(-16, -225),   S(14, -230),    S(30, -118),    S(37, -42),     S(37, -75),     S(34, -49),     S(6, -6),       //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(133, -594),   S(491, -1),     S(296, 171),    S(17, 132),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(356, -188),   S(383, 99),     S(205, 171),    S(22, 132),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(189, 41),     S(228, 155),    S(122, 186),    S(-32, 164),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(7, 12),       S(91, 116),     S(-25, 168),    S(-83, 180),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-117, -1),    S(-30, 78),     S(-95, 147),    S(-171, 192),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-51, -30),    S(13, 38),      S(-69, 117),    S(-121, 161),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(21, -81),     S(29, -4),      S(-33, 59),     S(-108, 112),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-21, -195),   S(3, -79),      S(-80, -15),    S(-75, -37),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const PParam KS_NO_QUEEN = S(-86, -435);

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(8, -5), S(10, 10), S(10, 4), S(4, 1), S(3, -10),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(4, -7), S(4, 4), S(2, 3), S(3, 0), S(3, -1),
};

inline const PParam KS_FLANK_ATTACK = S(3, -2);
inline const PParam KS_FLANK_DEFENSE = S(-3, -0);
inline const PParam KS_FLANK_DOUBLE_ATTACK   = S(2, -0);
inline const PParam KS_FLANK_DOUBLE_DEFENSE  = S(-2, 2);

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(17, -19), S(-7, 6), S(-3, -9), S(8, -16), S(12, -28), S(5, -51), S(-6, -43), }},
  {{ S(2, 2), S(-22, 1), S(-17, 8), S(-10, 16), S(-7, 8), S(-9, -18), S(-19, -24), }},
  {{ S(-3, -2), S(-11, -10), S(-12, 18), S(-7, 17), S(-7, 11), S(-8, -16), S(-24, -40), }},
  {{ S(6, 7), S(-10, 17), S(-7, 38), S(-2, 42), S(-3, 38), S(4, 15), S(7, -14), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(8, 25), S(-5, 3), S(-8, 10), S(-5, 27), S(2, 46),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(5, 10), S(-45, -143), S(-10, -43), S(-0, 4), S(-2, 3), S(-6, 9), S(-5, 8), }},
  {{ S(9, -0), S(-25, -153), S(-6, -30), S(-4, 2), S(-1, -3), S(-7, 4), S(2, 2), }},
  {{ S(-0, 10), S(-10, -81), S(6, -15), S(-0, 9), S(-4, 10), S(-10, 18), S(-6, 20), }},
  {{ S(1, 0), S(3, -57), S(-2, 30), S(-3, 25), S(-6, 10), S(-12, 12), S(-10, 20), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1402, 867, -19, -5
);

inline VParam WINNABLE_PAWNS = V(-20);
inline VParam WINNABLE_SYM = V(100);
inline VParam WINNABLE_ASYM = V(83);
inline VParam WINNABLE_PAWN_ENDGAME = V(109);
inline VParam WINNABLE_BIAS = V(-371);

// Epoch duration: 7.28738s
// clang-format on
}  // namespace Clockwork
