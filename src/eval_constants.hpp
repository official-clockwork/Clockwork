#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(163, 161);
inline const PParam KNIGHT_MAT = S(513, 311);
inline const PParam BISHOP_MAT = S(458, 268);
inline const PParam ROOK_MAT   = S(548, 408);
inline const PParam QUEEN_MAT  = S(1082, 559);
inline const PParam TEMPO_VAL  = S(64, 13);

inline const PParam BISHOP_PAIR_VAL   = S(76, 162);
inline const PParam ROOK_OPEN_VAL     = S(113, -26);
inline const PParam ROOK_SEMIOPEN_VAL = S(40, 19);

inline const PParam DOUBLED_PAWN_VAL = S(-19, -85);

inline const PParam POTENTIAL_CHECKER_VAL = S(-66, -11);
inline const PParam OUTPOST_KNIGHT_VAL    = S(50, 40);
inline const PParam OUTPOST_BISHOP_VAL    = S(60, 26);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(48, 4);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(56, -24);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(37, 35);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(67, -47);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(20, 11), S(55, 36), S(71, 69), S(171, 158), S(480, 229), S(644, 683),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(65, 41), S(58, 34), S(75, 64), S(178, 114), S(592, -12),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-80, -97), S(-79, -78), S(-55, 2), S(24, 74), S(112, 207), S(303, 277),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(41, -35), S(36, -6), S(30, 18), S(29, 63), S(95, 131), S(204, 217),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(11, -34), S(1, 6), S(-1, -22), S(-5, -42), S(-17, -89), S(-272, -105),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(16, 110), S(-9, 89), S(-8, 35), S(-6, 8), S(4, 7), S(47, 2), S(8, -6),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-291, -11), S(-26, 21), S(-7, 49), S(35, 62), S(36, 81), S(44, 86), S(20, 80),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(12, -68), S(110, 87), S(165, 158), S(200, 188), S(249, 197), S(277, 229), S(314, 221), S(351, 222), S(398, 161),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-13, -124), S(55, 37), S(116, 79), S(144, 126), S(174, 155), S(192, 174), S(201, 191), S(220, 195), S(231, 207), S(251, 195), S(274, 185), S(333, 141), S(339, 139), S(407, 96),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(274, 97), S(194, 261), S(222, 278), S(242, 282), S(257, 291), S(262, 303), S(270, 311), S(280, 309), S(289, 314), S(300, 316), S(315, 314), S(327, 312), S(335, 311), S(356, 290), S(464, 194),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(464, 98), S(525, 231), S(575, 264), S(603, 360), S(614, 419), S(630, 450), S(634, 493), S(642, 499), S(645, 523), S(651, 532), S(655, 545), S(661, 549), S(672, 537), S(677, 539), S(681, 536), S(685, 531), S(690, 524), S(689, 524), S(701, 503), S(720, 485), S(735, 465), S(751, 431), S(779, 412), S(900, 299), S(948, 248), S(944, 239), S(864, 285), S(970, 211),
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(551, -262), S(147, -121), S(58, -45), S(39, -9), S(14, -8), S(-17, 6), S(-39, 30), S(-66, 44), S(-82, 20),
};

inline const std::array<PParam, 3> KNIGHT_KING_RING = {
    S(240, 230), S(356, 185), S(455, 125),
};
inline const std::array<PParam, 3> BISHOP_KING_RING = {
    S(479, 371), S(293, 234), S(160, 69),
};
inline const std::array<PParam, 5> ROOK_KING_RING = {
    S(322, 319), S(444, 316), S(483, 325), S(592, 372), S(752, 331),
};
inline const std::array<PParam, 6> QUEEN_KING_RING = {
    S(881, 745), S(595, 609), S(333, 462), S(141, 259), S(83, -7), S(24, -298),
};

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(-85, 41), S(27, -18), S(-226, -129), S(63, 36), S(-242, -154),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(-25, 19), S(-20, 19), S(-19, 15), S(-17, 10), S(-21, -13),
};

inline const PParam PAWN_THREAT_KNIGHT = S(244, 61);
inline const PParam PAWN_THREAT_BISHOP = S(219, 115);
inline const PParam PAWN_THREAT_ROOK   = S(205, 99);
inline const PParam PAWN_THREAT_QUEEN  = S(185, -41);

inline const PParam KNIGHT_THREAT_BISHOP = S(118, 73);
inline const PParam KNIGHT_THREAT_ROOK   = S(257, 15);
inline const PParam KNIGHT_THREAT_QUEEN  = S(156, -17);

inline const PParam BISHOP_THREAT_KNIGHT = S(116, 37);
inline const PParam BISHOP_THREAT_ROOK   = S(240, 71);
inline const PParam BISHOP_THREAT_QUEEN  = S(200, 55);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(4, -20), S(-3, -8), S(-4, -18), S(-10, -24), S(-16, -31), S(-22, -34), S(-25, -45), S(-30, -46), S(-38, -51),
};

inline const PParam ROOK_LINEUP = S(14, 67);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(289, 300),    S(158, 370),    S(266, 324),    S(302, 224),    S(355, 161),    S(268, 239),    S(215, 269),    S(328, 220),    //
    S(112, 187),    S(159, 236),    S(173, 182),    S(160, 101),    S(147, 76),     S(123, 115),    S(106, 157),    S(72, 171),     //
    S(84, 156),     S(89, 171),     S(96, 140),     S(99, 92),      S(91, 79),      S(61, 106),     S(45, 137),     S(27, 158),     //
    S(62, 114),     S(81, 144),     S(78, 126),     S(79, 85),      S(60, 78),      S(49, 100),     S(12, 140),     S(-3, 126),     //
    S(52, 89),      S(104, 102),    S(82, 151),     S(79, 107),     S(56, 101),     S(28, 105),     S(14, 116),     S(-6, 108),     //
    S(87, 87),      S(178, 91),     S(101, 129),    S(108, 126),    S(83, 115),     S(68, 100),     S(49, 124),     S(13, 123),     //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-287, -42),   S(-215, 183),   S(-451, 434),   S(95, 147),     S(21, 153),     S(-111, 133),   S(-373, 196),   S(-347, 42),    //
    S(115, 119),    S(164, 120),    S(248, 80),     S(226, 113),    S(218, 126),    S(148, 122),    S(128, 131),    S(99, 104),     //
    S(187, 88),     S(204, 131),    S(257, 148),    S(217, 165),    S(189, 175),    S(129, 184),    S(135, 126),    S(112, 117),    //
    S(241, 132),    S(226, 150),    S(232, 174),    S(203, 207),    S(222, 196),    S(190, 182),    S(191, 139),    S(187, 115),    //
    S(231, 119),    S(270, 108),    S(244, 152),    S(230, 174),    S(215, 172),    S(215, 161),    S(215, 111),    S(177, 112),    //
    S(175, 93),     S(208, 90),     S(188, 125),    S(208, 151),    S(210, 145),    S(158, 134),    S(158, 99),     S(124, 81),     //
    S(188, 83),     S(211, 68),     S(182, 88),     S(179, 110),    S(168, 114),    S(147, 70),     S(152, 82),     S(109, 21),     //
    S(127, 64),     S(165, 121),    S(189, 74),     S(210, 60),     S(190, 82),     S(141, 64),     S(134, 96),     S(79, 39),      //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-32, 184),    S(-64, 166),    S(-367, 244),   S(-228, 211),   S(-194, 220),   S(-290, 251),   S(-107, 220),   S(-26, 194),    //
    S(86, 109),     S(28, 173),     S(61, 131),     S(-10, 182),    S(-20, 185),    S(34, 163),     S(63, 145),     S(45, 134),     //
    S(138, 129),    S(175, 122),    S(165, 156),    S(129, 147),    S(95, 148),     S(95, 156),     S(136, 135),    S(114, 114),    //
    S(125, 107),    S(143, 127),    S(166, 136),    S(143, 176),    S(172, 163),    S(105, 149),    S(116, 122),    S(81, 116),     //
    S(142, 82),     S(153, 110),    S(159, 129),    S(168, 150),    S(154, 158),    S(119, 148),    S(79, 131),     S(87, 97),      //
    S(160, 84),     S(196, 90),     S(211, 107),    S(158, 145),    S(150, 129),    S(154, 127),    S(163, 103),    S(101, 109),    //
    S(144, 34),     S(229, 55),     S(187, 70),     S(156, 97),     S(136, 108),    S(145, 79),     S(145, 86),     S(134, 47),     //
    S(147, 46),     S(130, 96),     S(142, 119),    S(160, 77),     S(165, 65),     S(157, 124),    S(152, 79),     S(128, 72),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(414, 241),    S(450, 249),    S(399, 277),    S(408, 260),    S(402, 261),    S(347, 274),    S(368, 274),    S(365, 276),    //
    S(311, 295),    S(375, 284),    S(458, 261),    S(390, 286),    S(389, 289),    S(346, 296),    S(266, 325),    S(264, 324),    //
    S(287, 286),    S(418, 251),    S(446, 244),    S(420, 246),    S(379, 258),    S(333, 280),    S(328, 284),    S(251, 317),    //
    S(263, 275),    S(340, 274),    S(381, 257),    S(349, 258),    S(345, 267),    S(303, 286),    S(283, 288),    S(221, 302),    //
    S(224, 229),    S(295, 239),    S(277, 255),    S(261, 249),    S(250, 257),    S(238, 283),    S(208, 273),    S(182, 272),    //
    S(192, 201),    S(257, 199),    S(257, 222),    S(236, 218),    S(239, 212),    S(212, 248),    S(203, 227),    S(160, 242),    //
    S(108, 215),    S(225, 157),    S(238, 183),    S(240, 189),    S(229, 193),    S(218, 205),    S(196, 189),    S(168, 204),    //
    S(154, 205),    S(164, 228),    S(235, 190),    S(254, 178),    S(234, 197),    S(224, 211),    S(210, 203),    S(192, 225),    //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(459, 411),    S(548, 338),    S(521, 379),    S(436, 486),    S(453, 448),    S(445, 441),    S(484, 371),    S(373, 456),    //
    S(483, 442),    S(434, 538),    S(427, 554),    S(282, 620),    S(290, 612),    S(354, 571),    S(386, 485),    S(396, 471),    //
    S(416, 501),    S(516, 484),    S(437, 554),    S(403, 572),    S(355, 578),    S(372, 531),    S(438, 440),    S(413, 413),    //
    S(495, 401),    S(491, 469),    S(452, 507),    S(422, 561),    S(415, 561),    S(412, 497),    S(464, 404),    S(455, 376),    //
    S(479, 401),    S(489, 420),    S(462, 468),    S(430, 514),    S(435, 507),    S(431, 470),    S(445, 400),    S(458, 348),    //
    S(469, 320),    S(501, 353),    S(499, 422),    S(460, 404),    S(459, 394),    S(466, 415),    S(469, 356),    S(454, 331),    //
    S(449, 196),    S(494, 157),    S(490, 245),    S(498, 295),    S(482, 322),    S(482, 287),    S(450, 337),    S(451, 322),    //
    S(408, 232),    S(453, 40),     S(456, 71),     S(484, 158),    S(489, 240),    S(491, 185),    S(481, 214),    S(435, 265),    //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-420, -241),  S(14, 89),      S(163, 8),      S(-93, -5),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(164, -105),   S(231, 40),     S(189, 49),     S(109, -18),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(78, 39),      S(251, 56),     S(231, 44),     S(86, 15),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-187, 60),    S(155, 37),     S(59, 39),      S(14, 31),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-212, 35),    S(82, 11),      S(84, -1),      S(-40, 40),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-95, 19),     S(156, -15),    S(106, -6),     S(41, 11),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(9, -17),      S(132, -15),    S(81, -16),     S(-2, 6),       S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-126, -33),   S(-9, -7),      S(-87, -11),    S(-97, -50),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(-57, 29), S(5, -32), S(55, -44), S(30, -25), S(34, -25), S(53, -44), S(10, -41), }},
  {{ S(-63, 30), S(51, -20), S(65, -44), S(11, 3), S(14, -10), S(68, -54), S(49, -30), }},
  {{ S(-49, 25), S(54, -15), S(-8, -28), S(-6, -5), S(-5, -12), S(-11, -37), S(53, -19), }},
  {{ S(-45, 13), S(21, 31), S(11, 7), S(8, 11), S(15, 5), S(20, 0), S(21, 6), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(-11, -85), S(40, -23), S(18, -14), S(-9, -9), S(16, 18),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(-25, -6), S(8, -9), S(16, -12), S(6, 1), S(13, 8), S(7, -6), S(7, -8), }},
  {{ S(-34, 5), S(-13, -10), S(11, -8), S(-6, 26), S(4, 19), S(11, -10), S(-13, -14), }},
  {{ S(4, -9), S(28, -37), S(15, -12), S(2, 4), S(8, 8), S(17, -11), S(24, -37), }},
  {{ S(-3, 16), S(14, 11), S(35, 3), S(8, -5), S(6, -4), S(34, 7), S(27, 6), }},
}};
// Epoch duration: 5.20419s
// clang-format on
}  // namespace Clockwork
