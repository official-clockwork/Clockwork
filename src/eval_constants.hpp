#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(171, 156);
inline const PParam KNIGHT_MAT = S(517, 316);
inline const PParam BISHOP_MAT = S(473, 272);
inline const PParam ROOK_MAT   = S(546, 411);
inline const PParam QUEEN_MAT  = S(1082, 561);
inline const PParam TEMPO_VAL  = S(63, 14);

inline const PParam BISHOP_PAIR_VAL   = S(77, 163);
inline const PParam ROOK_OPEN_VAL     = S(114, -26);
inline const PParam ROOK_SEMIOPEN_VAL = S(44, 17);

inline const PParam DOUBLED_PAWN_VAL = S(-32, -75);

inline const PParam POTENTIAL_CHECKER_VAL = S(-69, -10);
inline const PParam OUTPOST_KNIGHT_VAL    = S(51, 40);
inline const PParam OUTPOST_BISHOP_VAL    = S(61, 26);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(49, 4);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(58, -25);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(38, 34);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(70, -53);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(22, 12), S(57, 33), S(73, 70), S(181, 153), S(481, 225), S(668, 679),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(70, 38), S(60, 33), S(73, 65), S(175, 117), S(593, -10),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-80, -96), S(-80, -74), S(-54, 7), S(28, 76), S(102, 197), S(302, 277),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(43, -38), S(38, -6), S(29, 20), S(26, 66), S(97, 129), S(211, 209),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(10, -33), S(-1, 11), S(-4, -19), S(-7, -40), S(-14, -89), S(-268, -106),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-3, 114), S(-16, 90), S(-9, 33), S(-1, 5), S(11, 6), S(57, 2), S(19, -6),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-300, -9), S(-35, 23), S(-13, 50), S(34, 60), S(39, 80), S(48, 87), S(19, 84),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(15, -60), S(114, 94), S(171, 164), S(209, 193), S(255, 203), S(284, 235), S(320, 228), S(358, 229), S(404, 169),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-18, -122), S(53, 37), S(115, 79), S(143, 127), S(172, 156), S(190, 176), S(199, 192), S(217, 197), S(229, 208), S(249, 197), S(272, 187), S(331, 143), S(336, 142), S(407, 96),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(275, 98), S(198, 259), S(226, 279), S(243, 282), S(256, 292), S(262, 304), S(270, 312), S(281, 311), S(288, 316), S(299, 318), S(311, 317), S(321, 316), S(327, 315), S(347, 295), S(455, 200),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(457, 121), S(522, 233), S(570, 270), S(598, 364), S(609, 423), S(626, 454), S(630, 496), S(639, 501), S(642, 525), S(648, 534), S(651, 548), S(656, 551), S(668, 539), S(673, 542), S(677, 538), S(680, 534), S(686, 526), S(685, 526), S(697, 505), S(716, 487), S(731, 466), S(747, 433), S(775, 413), S(895, 301), S(946, 248), S(940, 241), S(861, 287), S(958, 217),
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(657, -331), S(243, -187), S(120, -84), S(81, -36), S(29, -21), S(-27, 4), S(-74, 40), S(-127, 68), S(-165, 56),
};

inline const std::array<PParam, 3> KNIGHT_KING_RING = {
    S(225, 227), S(344, 180), S(447, 118),
};
inline const std::array<PParam, 3> BISHOP_KING_RING = {
    S(457, 372), S(281, 235), S(163, 70),
};
inline const std::array<PParam, 5> ROOK_KING_RING = {
    S(319, 321), S(446, 313), S(488, 321), S(589, 372), S(746, 331),
};
inline const std::array<PParam, 6> QUEEN_KING_RING = {
    S(874, 747), S(589, 609), S(332, 459), S(142, 254), S(81, -12), S(15, -297),
};

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(-119, 61), S(27, -18), S(-216, -128), S(56, 38), S(-242, -154),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(-28, 22), S(-21, 20), S(-20, 15), S(-15, 9), S(-21, -12),
};

inline const PParam PAWN_THREAT_KNIGHT = S(245, 60);
inline const PParam PAWN_THREAT_BISHOP = S(217, 116);
inline const PParam PAWN_THREAT_ROOK   = S(203, 100);
inline const PParam PAWN_THREAT_QUEEN  = S(186, -45);

inline const PParam KNIGHT_THREAT_BISHOP = S(118, 73);
inline const PParam KNIGHT_THREAT_ROOK   = S(256, 15);
inline const PParam KNIGHT_THREAT_QUEEN  = S(155, -18);

inline const PParam BISHOP_THREAT_KNIGHT = S(115, 38);
inline const PParam BISHOP_THREAT_ROOK   = S(238, 72);
inline const PParam BISHOP_THREAT_QUEEN  = S(199, 52);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(3, -20), S(-3, -8), S(-4, -18), S(-9, -25), S(-15, -32), S(-21, -35), S(-24, -46), S(-29, -47), S(-37, -52),
};

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(294, 277),    S(196, 348),    S(309, 313),    S(292, 250),    S(343, 180),    S(249, 255),    S(203, 279),    S(309, 230),    //
    S(158, 173),    S(209, 218),    S(199, 165),    S(163, 130),    S(146, 100),    S(116, 140),    S(95, 179),     S(60, 185),     //
    S(100, 138),    S(99, 157),     S(112, 116),    S(98, 108),     S(83, 96),      S(53, 110),     S(24, 143),     S(9, 161),      //
    S(87, 93),      S(100, 123),    S(105, 96),     S(87, 99),      S(58, 93),      S(40, 100),     S(-9, 142),     S(-23, 128),    //
    S(85, 68),      S(145, 73),     S(91, 119),     S(75, 119),     S(43, 113),     S(12, 112),     S(-12, 127),    S(-28, 113),    //
    S(83, 75),      S(209, 84),     S(150, 129),    S(104, 146),    S(73, 131),     S(49, 117),     S(29, 136),     S(-9, 129),     //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-288, -49),   S(-224, 184),   S(-461, 436),   S(89, 146),     S(17, 153),     S(-113, 131),   S(-375, 194),   S(-343, 30),    //
    S(107, 119),    S(159, 120),    S(239, 81),     S(214, 116),    S(211, 128),    S(145, 121),    S(127, 129),    S(101, 103),    //
    S(176, 90),     S(189, 137),    S(246, 151),    S(207, 169),    S(181, 174),    S(124, 184),    S(135, 123),    S(105, 118),    //
    S(235, 131),    S(219, 149),    S(222, 175),    S(197, 208),    S(215, 196),    S(188, 181),    S(189, 137),    S(186, 112),    //
    S(228, 116),    S(263, 106),    S(239, 150),    S(225, 173),    S(211, 171),    S(213, 160),    S(214, 109),    S(176, 110),    //
    S(173, 90),     S(203, 87),     S(185, 124),    S(206, 148),    S(208, 144),    S(157, 132),    S(157, 96),     S(123, 79),     //
    S(184, 84),     S(199, 70),     S(180, 85),     S(178, 106),    S(165, 112),    S(145, 67),     S(150, 79),     S(110, 15),     //
    S(127, 62),     S(161, 120),    S(186, 73),     S(203, 61),     S(188, 80),     S(138, 61),     S(128, 96),     S(78, 34),      //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-34, 184),    S(-69, 168),    S(-360, 241),   S(-223, 208),   S(-190, 217),   S(-291, 252),   S(-105, 219),   S(-30, 194),    //
    S(76, 114),     S(24, 174),     S(58, 132),     S(-6, 180),     S(-21, 186),    S(38, 161),     S(67, 144),     S(50, 134),     //
    S(143, 126),    S(170, 124),    S(166, 156),    S(128, 149),    S(99, 146),     S(97, 155),     S(143, 132),    S(117, 113),    //
    S(122, 107),    S(142, 125),    S(162, 138),    S(142, 176),    S(171, 163),    S(111, 147),    S(118, 121),    S(89, 112),     //
    S(139, 81),     S(149, 109),    S(159, 128),    S(166, 150),    S(155, 158),    S(120, 148),    S(84, 129),     S(90, 96),      //
    S(157, 86),     S(192, 92),     S(210, 107),    S(158, 144),    S(150, 130),    S(156, 126),    S(165, 102),    S(104, 108),    //
    S(141, 38),     S(225, 56),     S(188, 69),     S(155, 96),     S(137, 106),    S(145, 79),     S(146, 85),     S(135, 45),     //
    S(147, 48),     S(133, 97),     S(138, 120),    S(157, 76),     S(165, 64),     S(156, 123),    S(151, 79),     S(130, 71),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(399, 246),    S(435, 254),    S(396, 278),    S(406, 261),    S(402, 260),    S(351, 271),    S(365, 273),    S(362, 276),    //
    S(293, 302),    S(356, 292),    S(447, 266),    S(382, 289),    S(385, 290),    S(345, 295),    S(264, 324),    S(262, 324),    //
    S(280, 288),    S(407, 255),    S(439, 247),    S(423, 245),    S(383, 254),    S(335, 279),    S(336, 280),    S(256, 313),    //
    S(258, 275),    S(334, 275),    S(374, 259),    S(354, 256),    S(349, 264),    S(307, 284),    S(288, 285),    S(228, 298),    //
    S(216, 230),    S(287, 240),    S(267, 258),    S(265, 245),    S(255, 255),    S(241, 282),    S(210, 273),    S(185, 270),    //
    S(184, 204),    S(248, 201),    S(252, 224),    S(238, 216),    S(243, 210),    S(214, 247),    S(205, 225),    S(164, 239),    //
    S(96, 220),     S(213, 163),    S(235, 184),    S(243, 187),    S(235, 188),    S(222, 202),    S(200, 185),    S(172, 201),    //
    S(146, 208),    S(163, 229),    S(236, 189),    S(263, 173),    S(243, 191),    S(231, 206),    S(216, 198),    S(197, 222),    //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(462, 412),    S(540, 347),    S(530, 380),    S(448, 484),    S(462, 447),    S(451, 442),    S(488, 372),    S(389, 447),    //
    S(484, 441),    S(427, 545),    S(432, 557),    S(289, 620),    S(295, 613),    S(364, 567),    S(393, 484),    S(414, 461),    //
    S(422, 497),    S(514, 485),    S(440, 557),    S(407, 576),    S(360, 579),    S(377, 532),    S(452, 435),    S(428, 406),    //
    S(498, 395),    S(494, 464),    S(454, 508),    S(430, 559),    S(419, 562),    S(421, 496),    S(475, 402),    S(473, 369),    //
    S(481, 398),    S(491, 417),    S(466, 467),    S(438, 511),    S(441, 507),    S(438, 469),    S(458, 398),    S(469, 349),    //
    S(469, 318),    S(503, 350),    S(504, 420),    S(469, 401),    S(465, 393),    S(472, 415),    S(477, 358),    S(463, 332),    //
    S(450, 199),    S(493, 152),    S(496, 242),    S(508, 290),    S(490, 318),    S(490, 286),    S(460, 336),    S(461, 322),    //
    S(408, 240),    S(458, 41),     S(459, 67),     S(487, 158),    S(495, 233),    S(496, 185),    S(485, 217),    S(437, 266),    //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-603, -152),  S(-132, 154),   S(58, 72),      S(-134, 40),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(23, -42),     S(174, 59),     S(173, 68),     S(141, -12),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-42, 87),     S(210, 62),     S(228, 54),     S(123, 19),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-293, 102),   S(132, 36),     S(63, 46),      S(56, 34),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-298, 68),    S(74, 2),       S(94, 5),       S(10, 39),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-153, 36),    S(160, -34),    S(119, -7),     S(90, 12),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-4, -25),     S(180, -56),    S(134, -28),    S(62, 7),       S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-189, -17),   S(-18, -21),    S(-107, 11),    S(-110, -4),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
    { S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0) },
    { S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0) },
    { S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0) },
    { S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0), S(0,0) },
}};
// Epoch duration: 8.68854s
  // clang-format on
}  // namespace Clockwork
