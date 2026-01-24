#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(164, 157);
inline const PParam KNIGHT_MAT = S(569, 307);
inline const PParam BISHOP_MAT = S(518, 268);
inline const PParam ROOK_MAT   = S(546, 381);
inline const PParam QUEEN_MAT  = S(1115, 519);
inline const PParam TEMPO_VAL  = S(65, 14);

inline const PParam BISHOP_PAIR_VAL   = S(78, 156);
inline const PParam ROOK_OPEN_VAL     = S(112, -26);
inline const PParam ROOK_SEMIOPEN_VAL = S(41, 19);

inline const PParam DOUBLED_PAWN_VAL = S(-19, -84);

inline const PParam POTENTIAL_CHECKER_VAL = S(-68, -11);
inline const PParam OUTPOST_KNIGHT_VAL    = S(49, 43);
inline const PParam OUTPOST_BISHOP_VAL    = S(59, 28);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(51, 4);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(60, -25);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(37, 37);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(68, -48);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(19, 13), S(56, 34), S(73, 66), S(161, 155), S(484, 185), S(659, 637),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(68, 40), S(58, 35), S(73, 65), S(165, 118), S(583, -19),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-87, -99), S(-84, -77), S(-57, 3), S(17, 79), S(121, 204), S(315, 275),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(44, -36), S(36, -6), S(29, 17), S(25, 65), S(94, 133), S(178, 229),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(15, -34), S(2, 3), S(3, -21), S(-4, -41), S(-21, -88), S(-286, -100),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(11, 113), S(-7, 89), S(-7, 35), S(-3, 6), S(3, 4), S(41, -0), S(15, -11),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-313, -11), S(-28, 18), S(-9, 50), S(33, 64), S(39, 83), S(48, 89), S(23, 83),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(15, -114), S(105, 56), S(151, 130), S(184, 165), S(231, 175), S(261, 208), S(297, 202), S(335, 205), S(383, 144),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(1, -182), S(58, 13), S(122, 72), S(155, 115), S(181, 147), S(200, 168), S(209, 184), S(226, 190), S(237, 202), S(259, 191), S(278, 183), S(331, 144), S(343, 141), S(407, 95),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(273, 75), S(204, 248), S(231, 267), S(252, 272), S(266, 284), S(272, 295), S(280, 304), S(290, 303), S(298, 309), S(310, 309), S(325, 307), S(336, 305), S(346, 303), S(362, 287), S(458, 198),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(446, 24), S(555, 118), S(588, 213), S(610, 329), S(625, 386), S(637, 430), S(644, 466), S(653, 474), S(657, 500), S(663, 510), S(668, 519), S(675, 521), S(684, 515), S(688, 518), S(693, 512), S(696, 508), S(699, 504), S(703, 499), S(711, 485), S(726, 470), S(742, 446), S(769, 414), S(803, 379), S(895, 288), S(950, 236), S(1001, 204), S(1021, 178), S(977, 191),
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(592, -221), S(143, -113), S(56, -38), S(41, -10), S(17, -9), S(-17, 4), S(-39, 25), S(-72, 42), S(-97, 21),
};

inline const std::array<PParam, 3> KNIGHT_KING_RING = {
    S(229, 227), S(373, 169), S(500, 91),
};
inline const std::array<PParam, 3> BISHOP_KING_RING = {
    S(428, 349), S(264, 220), S(153, 63),
};
inline const std::array<PParam, 5> ROOK_KING_RING = {
    S(322, 315), S(453, 307), S(503, 307), S(626, 345), S(797, 295),
};
inline const std::array<PParam, 6> QUEEN_KING_RING = {
    S(913, 724), S(610, 592), S(337, 440), S(127, 235), S(55, -38), S(-10, -328),
};

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(-91, 42), S(51, -34), S(-207, -121), S(74, 27), S(-257, -154),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(-26, 20), S(-21, 18), S(-20, 16), S(-16, 9), S(-22, -12),
};

inline const PParam PAWN_THREAT_KNIGHT = S(243, 66);
inline const PParam PAWN_THREAT_BISHOP = S(218, 117);
inline const PParam PAWN_THREAT_ROOK   = S(215, 86);
inline const PParam PAWN_THREAT_QUEEN  = S(190, -47);

inline const PParam KNIGHT_THREAT_BISHOP = S(124, 73);
inline const PParam KNIGHT_THREAT_ROOK   = S(256, 18);
inline const PParam KNIGHT_THREAT_QUEEN  = S(168, -38);

inline const PParam BISHOP_THREAT_KNIGHT = S(121, 38);
inline const PParam BISHOP_THREAT_ROOK   = S(248, 72);
inline const PParam BISHOP_THREAT_QUEEN  = S(206, 47);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(4, -22), S(-3, -9), S(-5, -18), S(-11, -25), S(-17, -31), S(-23, -35), S(-26, -44), S(-32, -44), S(-45, -41),
};

inline const PParam ROOK_LINEUP = S(16, 65);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(329, 291),    S(197, 364),    S(284, 327),    S(321, 222),    S(353, 168),    S(282, 227),    S(219, 266),    S(315, 219),    //
    S(118, 188),    S(164, 232),    S(183, 183),    S(160, 102),    S(146, 83),     S(131, 113),    S(117, 154),    S(67, 172),     //
    S(97, 154),     S(103, 166),    S(106, 140),    S(108, 85),     S(101, 74),     S(73, 105),     S(49, 139),     S(28, 158),     //
    S(69, 113),     S(87, 142),     S(82, 129),     S(83, 81),      S(65, 76),      S(55, 101),     S(16, 139),     S(-2, 129),     //
    S(57, 89),      S(109, 104),    S(86, 149),     S(83, 101),     S(57, 99),      S(33, 103),     S(19, 118),     S(-6, 111),     //
    S(93, 88),      S(182, 90),     S(101, 132),    S(119, 119),    S(90, 115),     S(73, 98),      S(52, 124),     S(14, 125),     //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-325, -12),   S(-217, 174),   S(-314, 245),   S(54, 138),     S(-5, 133),     S(-137, 130),   S(-316, 172),   S(-381, 64),    //
    S(76, 113),     S(143, 110),    S(220, 75),     S(208, 101),    S(191, 118),    S(134, 105),    S(97, 124),     S(60, 100),     //
    S(164, 85),     S(190, 119),    S(227, 139),    S(201, 146),    S(168, 158),    S(123, 161),    S(132, 109),    S(92, 107),     //
    S(216, 114),    S(217, 136),    S(220, 157),    S(200, 185),    S(214, 180),    S(170, 168),    S(173, 131),    S(159, 108),    //
    S(222, 105),    S(251, 97),     S(232, 137),    S(223, 156),    S(204, 158),    S(200, 143),    S(192, 102),    S(162, 102),    //
    S(156, 86),     S(187, 81),     S(177, 116),    S(191, 139),    S(191, 135),    S(148, 123),    S(141, 87),     S(107, 67),     //
    S(168, 71),     S(188, 70),     S(160, 79),     S(167, 99),     S(157, 99),     S(126, 67),     S(123, 76),     S(88, 18),      //
    S(99, 58),      S(146, 110),    S(166, 67),     S(181, 64),     S(165, 80),     S(127, 61),     S(113, 88),     S(61, 35),      //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-60, 187),    S(-80, 177),    S(-332, 223),   S(-232, 200),   S(-218, 219),   S(-287, 233),   S(-146, 226),   S(-39, 193),    //
    S(73, 102),     S(34, 160),     S(51, 133),     S(-12, 169),    S(-7, 170),     S(34, 154),     S(70, 140),     S(41, 126),     //
    S(125, 123),    S(153, 131),    S(155, 145),    S(132, 135),    S(101, 135),    S(112, 135),    S(132, 126),    S(106, 107),    //
    S(118, 104),    S(143, 122),    S(151, 139),    S(145, 169),    S(179, 151),    S(108, 138),    S(116, 109),    S(77, 109),     //
    S(135, 73),     S(159, 99),     S(164, 122),    S(166, 143),    S(148, 150),    S(124, 138),    S(91, 116),     S(83, 84),      //
    S(161, 77),     S(200, 83),     S(215, 103),    S(160, 135),    S(152, 118),    S(148, 121),    S(154, 97),     S(94, 103),     //
    S(136, 38),     S(229, 50),     S(189, 65),     S(159, 88),     S(138, 96),     S(140, 71),     S(138, 72),     S(128, 60),     //
    S(154, 60),     S(141, 85),     S(154, 101),    S(163, 65),     S(161, 68),     S(158, 115),    S(151, 91),     S(132, 83),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(415, 239),    S(454, 240),    S(414, 262),    S(430, 247),    S(423, 240),    S(388, 248),    S(380, 258),    S(369, 267),    //
    S(294, 301),    S(385, 282),    S(457, 262),    S(397, 281),    S(386, 286),    S(365, 287),    S(296, 313),    S(273, 319),    //
    S(283, 288),    S(403, 254),    S(445, 247),    S(422, 243),    S(385, 259),    S(340, 281),    S(336, 279),    S(261, 316),    //
    S(268, 273),    S(352, 270),    S(384, 259),    S(363, 258),    S(358, 262),    S(314, 283),    S(302, 281),    S(231, 303),    //
    S(233, 233),    S(296, 241),    S(293, 254),    S(269, 256),    S(256, 265),    S(245, 281),    S(215, 275),    S(195, 274),    //
    S(208, 196),    S(270, 203),    S(267, 223),    S(252, 220),    S(248, 219),    S(223, 245),    S(212, 230),    S(177, 239),    //
    S(116, 213),    S(232, 159),    S(249, 180),    S(249, 191),    S(238, 195),    S(228, 205),    S(210, 189),    S(179, 207),    //
    S(166, 208),    S(189, 221),    S(252, 187),    S(267, 179),    S(257, 188),    S(239, 206),    S(225, 204),    S(204, 222),    //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(499, 367),    S(550, 336),    S(505, 391),    S(444, 467),    S(450, 437),    S(458, 420),    S(494, 354),    S(421, 419),    //
    S(487, 427),    S(463, 501),    S(448, 525),    S(297, 608),    S(304, 596),    S(378, 550),    S(414, 461),    S(420, 444),    //
    S(437, 475),    S(515, 460),    S(449, 535),    S(412, 551),    S(381, 550),    S(397, 512),    S(452, 429),    S(439, 397),    //
    S(505, 379),    S(514, 436),    S(465, 490),    S(451, 526),    S(443, 527),    S(434, 473),    S(477, 397),    S(460, 377),    //
    S(499, 369),    S(507, 398),    S(480, 449),    S(445, 496),    S(445, 494),    S(445, 456),    S(454, 401),    S(470, 336),    //
    S(479, 304),    S(516, 337),    S(514, 396),    S(475, 396),    S(473, 391),    S(475, 407),    S(482, 345),    S(459, 332),    //
    S(457, 188),    S(493, 168),    S(498, 240),    S(510, 281),    S(496, 312),    S(493, 282),    S(460, 328),    S(461, 320),    //
    S(424, 211),    S(458, 54),     S(481, 64),     S(503, 155),    S(505, 222),    S(501, 184),    S(489, 217),    S(447, 271),    //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-419, -256),  S(56, 95),      S(118, 30),     S(-189, 41),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(123, -93),    S(233, 71),     S(177, 59),     S(83, 0),       S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(39, 45),      S(271, 49),     S(240, 34),     S(100, 4),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-153, 47),    S(182, 27),     S(83, 26),      S(-1, 33),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-204, 31),    S(95, 6),       S(84, -5),      S(-54, 44),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-93, 16),     S(169, -18),    S(119, -10),    S(42, 15),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(5, -15),      S(136, -11),    S(83, -13),     S(4, 10),       S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-132, -37),   S(-13, -6),     S(-86, -14),    S(-110, -42),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(-62, 31), S(2, -37), S(49, -42), S(28, -22), S(32, -24), S(52, -45), S(7, -45), }},
  {{ S(-64, 30), S(56, -24), S(66, -48), S(13, 0), S(14, -10), S(68, -60), S(56, -37), }},
  {{ S(-52, 27), S(57, -17), S(-9, -31), S(-11, -6), S(-9, -13), S(-7, -39), S(59, -24), }},
  {{ S(-47, 12), S(16, 27), S(8, 9), S(3, 12), S(12, 4), S(17, -2), S(15, 9), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(-2, -90), S(45, -24), S(18, -13), S(-12, -4), S(-2, 31),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(-20, -7), S(6, -7), S(12, -8), S(4, 7), S(11, 15), S(8, -4), S(7, -7), }},
  {{ S(-39, 15), S(-19, 1), S(8, -2), S(-7, 35), S(1, 30), S(8, -3), S(-20, -1), }},
  {{ S(8, -6), S(25, -30), S(18, -11), S(4, 7), S(6, 12), S(19, -7), S(23, -31), }},
  {{ S(-0, 19), S(22, 10), S(33, 7), S(6, 1), S(5, -1), S(32, 11), S(26, 13), }},
}};
// Epoch duration: 9.05449s
// clang-format on
}  // namespace Clockwork
