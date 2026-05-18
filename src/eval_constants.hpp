#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(185, 470);
inline const PParam KNIGHT_MAT = S(785, 1489);
inline const PParam BISHOP_MAT = S(823, 1583);
inline const PParam ROOK_MAT   = S(987, 2514);
inline const PParam QUEEN_MAT  = S(2315, 3791);
inline const PParam TEMPO_VAL  = S(61, 24);

inline const PParam BISHOP_XRAY_PAWNS   = S(-13, 13);
inline const PParam BISHOP_PAIR_VAL   = S(61, 223);
inline const PParam ROOK_OPEN_VAL     = S(118, -11);
inline const PParam ROOK_SEMIOPEN_VAL = S(45, 32);
inline const PParam MINOR_BEHIND_PAWN = S(16, 40);
inline const PParam RESTRICTED_SQUARES = S(-23, -7);

inline const PParam DOUBLED_PAWN_VAL = S(-21, -72);
inline const PParam ISOLATED_PAWN_VAL = S(-13, -38);

inline const PParam POTENTIAL_CHECKER_VAL = S(-44, -40);
inline const PParam OUTPOST_KNIGHT_VAL    = S(57, 53);
inline const PParam OUTPOST_BISHOP_VAL    = S(57, 38);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(35, 11);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(41, -8);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(22, 60);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(58, -39);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(13, -1), S(28, 36), S(57, 67), S(124, 196), S(416, 290), S(575, 614),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(45, 44), S(41, 39), S(60, 72), S(103, 201), S(408, 170),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-90, -252), S(-87, -216), S(-67, -84), S(-25, 44), S(71, 225), S(219, 355),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(24, -40), S(23, -5), S(13, 34), S(10, 112), S(73, 239), S(112, 461),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(18, -34), S(6, 10), S(-4, -20), S(-1, -51), S(-9, -159), S(-180, -308),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(17, 216), S(-3, 174), S(-3, 98), S(6, 59), S(18, 61), S(57, 60), S(47, 43),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-103, -164), S(15, 21), S(2, 110), S(26, 157), S(36, 192), S(48, 199), S(29, 186),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-83, -360), S(-23, -150), S(5, -34), S(28, 10), S(55, 38), S(70, 74), S(88, 81), S(106, 90), S(124, 43),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-54, -296), S(-8, -133), S(39, -63), S(59, -7), S(81, 34), S(93, 62), S(98, 86), S(101, 105), S(104, 116), S(108, 120), S(115, 110), S(127, 92), S(129, 93), S(101, 68),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(111, -194), S(27, -20), S(47, 13), S(63, 27), S(73, 45), S(77, 59), S(82, 73), S(88, 78), S(93, 89), S(101, 94), S(108, 98), S(111, 103), S(110, 107), S(112, 91), S(136, 40),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-212, -143), S(-70, -33), S(-44, 86), S(-23, 205), S(-8, 247), S(-1, 296), S(5, 329), S(11, 344), S(14, 368), S(16, 387), S(21, 395), S(23, 406), S(26, 409), S(26, 416), S(25, 420), S(19, 426), S(15, 427), S(16, 417), S(16, 412), S(19, 403), S(12, 390), S(16, 365), S(-12, 387), S(-55, 373), S(-86, 376), S(-123, 399), S(-163, 407), S(-136, 335),
};

inline const PParam PAWN_THREAT_KNIGHT = S(195, 143);
inline const PParam PAWN_THREAT_BISHOP = S(182, 192);
inline const PParam PAWN_THREAT_ROOK   = S(197, 150);
inline const PParam PAWN_THREAT_QUEEN  = S(168, 24);

inline const PParam KNIGHT_THREAT_BISHOP = S(109, 121);
inline const PParam KNIGHT_THREAT_ROOK   = S(222, 94);
inline const PParam KNIGHT_THREAT_QUEEN  = S(160, -20);

inline const PParam BISHOP_THREAT_KNIGHT = S(95, 82);
inline const PParam BISHOP_THREAT_ROOK   = S(207, 160);
inline const PParam BISHOP_THREAT_QUEEN  = S(176, 105);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(-0, -10), S(2, -14), S(-1, -22), S(-5, -32), S(-10, -42), S(-15, -48), S(-16, -61), S(-21, -64), S(-24, -80),
};

inline const PParam ROOK_LINEUP = S(16, 76);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(189, 241),    S(42, 341),     S(72, 317),     S(137, 229),    S(193, 138),    S(118, 204),    S(83, 242),     S(229, 183),    //
    S(36, 41),      S(47, 75),      S(26, 23),      S(39, -41),     S(34, -77),     S(4, -37),      S(-1, 14),      S(-25, 44),     //
    S(27, -22),     S(4, -14),      S(33, -56),     S(20, -79),     S(12, -98),     S(-8, -82),     S(-42, -49),    S(-46, -10),    //
    S(-13, -75),    S(-35, -43),    S(-1, -64),     S(-11, -76),    S(-30, -88),    S(-39, -78),    S(-86, -58),    S(-80, -56),    //
    S(-15, -104),   S(20, -103),    S(7, -44),      S(-9, -50),     S(-35, -63),    S(-49, -72),    S(-77, -71),    S(-81, -75),    //
    S(9, -99),      S(83, -88),     S(70, -38),     S(24, -13),     S(-4, -37),     S(-23, -60),    S(-46, -56),    S(-60, -56),    //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-259, -497),  S(-281, 57),    S(-295, -88),   S(-10, 26),     S(-117, 46),    S(-284, 72),    S(-409, 125),   S(-350, -371),  //
    S(-29, -17),    S(-19, 29),     S(76, 6),       S(61, 48),      S(74, 33),      S(35, 16),      S(-28, 20),     S(-46, 3),      //
    S(35, -25),     S(30, 25),      S(71, 57),      S(77, 76),      S(68, 80),      S(24, 69),      S(10, 17),      S(-41, 10),     //
    S(95, 17),      S(97, 50),      S(105, 70),     S(109, 108),    S(116, 102),    S(81, 73),      S(58, 40),      S(38, 27),      //
    S(72, 12),      S(106, 3),      S(101, 53),     S(90, 76),      S(78, 73),      S(79, 63),      S(64, 8),       S(23, 19),      //
    S(17, -44),     S(47, -15),     S(51, 20),      S(64, 49),      S(58, 46),      S(33, 13),      S(21, -12),     S(-23, -50),    //
    S(38, -33),     S(44, -29),     S(26, -26),     S(33, -3),      S(30, -6),      S(5, -53),      S(-28, -13),    S(-35, -116),   //
    S(-39, -92),    S(11, -38),     S(33, -60),     S(51, -47),     S(31, -28),     S(-5, -62),     S(-15, -39),    S(-58, -143),   //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-129, 23),    S(-190, 113),   S(-496, 195),   S(-320, 107),   S(-332, 137),   S(-345, 153),   S(-230, 135),   S(-129, 58),    //
    S(-48, -15),    S(-96, 85),     S(-58, 49),     S(-112, 79),    S(-100, 75),    S(-53, 45),     S(-29, 35),     S(-55, 13),     //
    S(29, 16),      S(10, 58),      S(23, 63),      S(19, 53),      S(16, 33),      S(13, 35),      S(13, 36),      S(15, -3),      //
    S(9, -5),       S(52, 12),      S(64, 33),      S(75, 60),      S(104, 48),     S(49, 19),      S(50, -12),     S(0, -11),      //
    S(52, -59),     S(50, -9),      S(83, 7),       S(95, 29),      S(82, 41),      S(72, 20),      S(24, 2),       S(21, -41),     //
    S(65, -38),     S(90, -28),     S(102, 1),      S(74, 22),      S(69, 4),       S(68, 10),      S(76, -11),     S(25, -27),     //
    S(38, -96),     S(119, -67),    S(78, -34),     S(55, -17),     S(40, -16),     S(56, -53),     S(56, -62),     S(50, -68),     //
    S(55, -89),     S(27, -29),     S(32, -16),     S(56, -40),     S(38, -31),     S(45, 2),       S(52, -28),     S(50, -67),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(201, 76),     S(234, 85),     S(178, 112),    S(170, 85),     S(236, 42),     S(189, 70),     S(213, 76),     S(193, 84),     //
    S(93, 158),     S(162, 148),    S(230, 110),    S(171, 125),    S(215, 110),    S(173, 133),    S(115, 155),    S(102, 159),    //
    S(59, 150),     S(193, 96),     S(215, 84),     S(201, 61),     S(198, 84),     S(136, 124),    S(139, 124),    S(73, 171),     //
    S(30, 115),     S(104, 127),    S(130, 99),     S(109, 99),     S(141, 86),     S(99, 128),     S(93, 135),     S(25, 156),     //
    S(3, 41),       S(78, 52),      S(65, 70),      S(39, 69),      S(49, 78),      S(37, 106),     S(14, 104),     S(-6, 102),     //
    S(13, -40),     S(89, -19),     S(76, 3),       S(55, 5),       S(66, 10),      S(37, 44),      S(43, 24),      S(-6, 36),      //
    S(-73, -45),    S(62, -86),     S(62, -51),     S(50, -17),     S(56, -19),     S(40, -6),      S(31, -22),     S(1, -15),      //
    S(-12, -77),    S(1, -37),      S(71, -55),     S(80, -54),     S(80, -44),     S(59, -22),     S(59, -35),     S(41, -29),     //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(26, 180),     S(123, 123),    S(27, 264),     S(-35, 365),    S(1, 326),      S(41, 229),     S(39, 154),     S(-29, 207),    //
    S(37, 181),     S(43, 256),     S(5, 328),      S(-142, 463),   S(-88, 433),    S(-31, 324),    S(22, 181),     S(5, 165),      //
    S(-10, 240),    S(18, 293),     S(-17, 370),    S(-62, 416),    S(-39, 399),    S(13, 284),     S(48, 178),     S(48, 99),      //
    S(10, 143),     S(-2, 249),     S(-37, 316),    S(-60, 395),    S(-41, 400),    S(0, 256),      S(44, 154),     S(23, 113),     //
    S(3, 128),      S(-7, 168),     S(-24, 232),    S(-46, 303),    S(-31, 327),    S(-5, 245),     S(6, 163),      S(34, 70),      //
    S(6, 23),       S(27, 50),      S(20, 134),     S(-3, 155),     S(6, 165),      S(11, 193),     S(33, 121),     S(24, 74),      //
    S(-21, -133),   S(8, -115),     S(9, -31),      S(32, 5),       S(25, 54),      S(30, 29),      S(4, 59),       S(22, 32),      //
    S(-34, -105),   S(-15, -258),   S(14, -255),    S(29, -146),    S(34, -60),     S(37, -97),     S(34, -67),     S(8, -29),      //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(145, -563),   S(491, -5),     S(323, 131),    S(62, 90),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(337, -154),   S(367, 119),    S(217, 157),    S(45, 121),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(178, 63),     S(213, 180),    S(118, 198),    S(-22, 164),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(2, 25),       S(73, 144),     S(-33, 179),    S(-91, 191),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-124, 8),     S(-49, 94),     S(-115, 155),   S(-188, 195),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-54, -27),    S(-0, 46),      S(-84, 120),    S(-129, 156),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(23, -85),     S(25, -5),      S(-36, 53),     S(-106, 100),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-19, -189),   S(4, -79),      S(-77, -20),    S(-69, -44),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const PParam KS_NO_QUEEN = S(-100, -435);

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(8, -7), S(9, 10), S(10, 5), S(4, 0), S(3, -10),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(4, -7), S(4, 3), S(3, 1), S(3, -0), S(3, -2),
};

inline const PParam KS_FLANK_ATTACK = S(3, -2);
inline const PParam KS_FLANK_DEFENSE = S(-4, 0);
inline const PParam KS_FLANK_DOUBLE_ATTACK   = S(2, -1);
inline const PParam KS_FLANK_DOUBLE_DEFENSE  = S(-2, 2);

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(18, -19), S(-6, 5), S(-2, -8), S(9, -16), S(14, -30), S(9, -55), S(-2, -47), }},
  {{ S(1, 3), S(-22, 4), S(-18, 11), S(-10, 17), S(-7, 8), S(-8, -20), S(-19, -24), }},
  {{ S(-2, -2), S(-10, -10), S(-12, 17), S(-7, 17), S(-6, 9), S(-6, -20), S(-22, -39), }},
  {{ S(7, 6), S(-9, 14), S(-6, 35), S(-2, 40), S(-2, 35), S(7, 8), S(11, -20), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(8, 28), S(-4, 4), S(-6, 10), S(-5, 28), S(2, 49),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(6, 10), S(-45, -148), S(-9, -49), S(1, 4), S(-1, 3), S(-5, 10), S(-4, 8), }},
  {{ S(11, -2), S(-26, -157), S(-5, -33), S(-2, 0), S(1, -4), S(-6, 3), S(3, 1), }},
  {{ S(1, 10), S(-10, -86), S(7, -18), S(1, 9), S(-2, 10), S(-9, 18), S(-5, 19), }},
  {{ S(3, 1), S(4, -69), S(-2, 30), S(-3, 27), S(-5, 10), S(-11, 13), S(-9, 20), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1409, 878, -16, -5
);

inline VParam WINNABLE_PAWNS = V(-21);
inline VParam WINNABLE_SYM = V(101);
inline VParam WINNABLE_ASYM = V(83);
inline VParam WINNABLE_PAWN_ENDGAME = V(121);
inline VParam WINNABLE_BIAS = V(-365);

// Epoch duration: 8.37148s
// clang-format on
}  // namespace Clockwork
