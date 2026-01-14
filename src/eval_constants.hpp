#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(162, 164);
inline const PParam KNIGHT_MAT = S(515, 314);
inline const PParam BISHOP_MAT = S(453, 266);
inline const PParam ROOK_MAT   = S(545, 405);
inline const PParam QUEEN_MAT  = S(1078, 555);
inline const PParam TEMPO_VAL  = S(63, 14);

inline const PParam BISHOP_PAIR_VAL   = S(77, 163);
inline const PParam ROOK_OPEN_VAL     = S(114, -27);
inline const PParam ROOK_SEMIOPEN_VAL = S(42, 17);

inline const PParam DOUBLED_PAWN_VAL = S(-15, -84);

inline const PParam POTENTIAL_CHECKER_VAL = S(-66, -11);
inline const PParam OUTPOST_KNIGHT_VAL    = S(51, 40);
inline const PParam OUTPOST_BISHOP_VAL    = S(60, 26);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(48, 4);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(57, -25);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(38, 34);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(69, -51);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(20, 10), S(52, 36), S(71, 70), S(175, 156), S(479, 223), S(661, 672),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(66, 39), S(56, 36), S(70, 66), S(172, 118), S(590, -10),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-77, -97), S(-79, -75), S(-54, 7), S(24, 79), S(100, 201), S(303, 275),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(42, -37), S(37, -6), S(30, 18), S(28, 63), S(96, 131), S(204, 216),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(10, -32), S(2, 6), S(-2, -23), S(-5, -41), S(-16, -87), S(-270, -103),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-8, 116), S(-16, 89), S(-5, 31), S(-0, 4), S(11, 3), S(58, -1), S(19, -9),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-309, -8), S(-33, 20), S(-9, 48), S(32, 62), S(37, 82), S(46, 87), S(18, 83),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(13, -68), S(111, 86), S(166, 157), S(202, 188), S(249, 197), S(277, 229), S(314, 221), S(352, 223), S(398, 162),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-16, -123), S(54, 37), S(117, 78), S(145, 126), S(174, 155), S(192, 175), S(201, 191), S(220, 196), S(231, 206), S(251, 195), S(274, 185), S(333, 141), S(339, 138), S(407, 96),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(282, 97), S(202, 261), S(231, 279), S(249, 283), S(264, 292), S(269, 304), S(276, 312), S(287, 311), S(295, 317), S(305, 318), S(319, 317), S(329, 315), S(335, 315), S(355, 294), S(463, 199),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(465, 123), S(531, 234), S(581, 267), S(609, 363), S(620, 422), S(637, 452), S(641, 496), S(649, 501), S(652, 525), S(658, 535), S(662, 548), S(667, 552), S(679, 540), S(684, 542), S(688, 539), S(692, 534), S(697, 527), S(697, 526), S(709, 505), S(729, 486), S(744, 466), S(760, 433), S(789, 412), S(909, 299), S(958, 247), S(954, 238), S(877, 283), S(974, 214),
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(600, -318), S(196, -173), S(90, -77), S(57, -29), S(19, -15), S(-26, 8), S(-58, 38), S(-96, 61), S(-123, 48),
};

inline const std::array<PParam, 3> KNIGHT_KING_RING = {
    S(238, 227), S(354, 182), S(451, 123),
};
inline const std::array<PParam, 3> BISHOP_KING_RING = {
    S(492, 374), S(304, 235), S(167, 71),
};
inline const std::array<PParam, 5> ROOK_KING_RING = {
    S(318, 319), S(446, 313), S(484, 322), S(592, 369), S(754, 327),
};
inline const std::array<PParam, 6> QUEEN_KING_RING = {
    S(875, 743), S(592, 607), S(333, 460), S(140, 258), S(81, -7), S(21, -296),
};

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(-100, 52), S(27, -17), S(-230, -129), S(62, 36), S(-241, -153),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(-26, 21), S(-20, 19), S(-18, 14), S(-16, 9), S(-22, -12),
};

inline const PParam PAWN_THREAT_KNIGHT = S(243, 61);
inline const PParam PAWN_THREAT_BISHOP = S(218, 115);
inline const PParam PAWN_THREAT_ROOK   = S(204, 99);
inline const PParam PAWN_THREAT_QUEEN  = S(185, -44);

inline const PParam KNIGHT_THREAT_BISHOP = S(118, 73);
inline const PParam KNIGHT_THREAT_ROOK   = S(256, 15);
inline const PParam KNIGHT_THREAT_QUEEN  = S(155, -18);

inline const PParam BISHOP_THREAT_KNIGHT = S(115, 38);
inline const PParam BISHOP_THREAT_ROOK   = S(238, 72);
inline const PParam BISHOP_THREAT_QUEEN  = S(199, 51);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(4, -20), S(-3, -8), S(-4, -18), S(-9, -25), S(-15, -31), S(-22, -34), S(-25, -45), S(-30, -46), S(-38, -50),
};

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(277, 298),    S(163, 359),    S(302, 305),    S(298, 226),    S(352, 163),    S(261, 246),    S(211, 273),    S(322, 222),    //
    S(114, 191),    S(165, 246),    S(177, 198),    S(169, 113),    S(154, 85),     S(130, 126),    S(110, 166),    S(71, 176),     //
    S(68, 153),     S(82, 174),     S(103, 198),    S(98, 94),      S(87, 85),      S(65, 99),      S(37, 133),     S(20, 153),     //
    S(50, 109),     S(74, 144),     S(89, 182),     S(80, 89),      S(58, 85),      S(51, 91),      S(3, 133),      S(-13, 122),    //
    S(39, 88),      S(99, 102),     S(67, 157),     S(75, 111),     S(48, 105),     S(25, 102),     S(3, 116),      S(-17, 106),    //
    S(73, 86),      S(183, 86),     S(154, 119),    S(102, 130),    S(75, 118),     S(60, 108),     S(42, 127),     S(1, 122),      //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-293, -43),   S(-220, 185),   S(-458, 436),   S(94, 147),     S(22, 153),     S(-110, 131),   S(-376, 197),   S(-348, 36),    //
    S(113, 118),    S(161, 121),    S(245, 80),     S(225, 112),    S(219, 126),    S(151, 120),    S(130, 130),    S(103, 104),    //
    S(182, 89),     S(195, 136),    S(255, 148),    S(216, 166),    S(189, 172),    S(129, 183),    S(138, 124),    S(112, 117),    //
    S(237, 133),    S(225, 149),    S(229, 173),    S(203, 207),    S(221, 195),    S(192, 182),    S(193, 138),    S(188, 115),    //
    S(229, 118),    S(266, 107),    S(242, 150),    S(229, 174),    S(214, 173),    S(217, 160),    S(216, 111),    S(178, 112),    //
    S(173, 93),     S(207, 88),     S(188, 124),    S(209, 149),    S(210, 146),    S(159, 134),    S(159, 99),     S(125, 81),     //
    S(187, 83),     S(208, 69),     S(182, 88),     S(180, 109),    S(169, 113),    S(147, 70),     S(152, 82),     S(110, 18),     //
    S(127, 64),     S(163, 121),    S(188, 74),     S(207, 62),     S(189, 82),     S(140, 63),     S(133, 96),     S(77, 38),      //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-44, 184),    S(-78, 168),    S(-372, 241),   S(-234, 208),   S(-199, 217),   S(-299, 250),   S(-115, 218),   S(-39, 194),    //
    S(73, 109),     S(16, 173),     S(49, 132),     S(-15, 179),    S(-27, 183),    S(28, 160),     S(58, 143),     S(43, 132),     //
    S(129, 127),    S(162, 123),    S(158, 154),    S(120, 147),    S(89, 145),     S(88, 154),     S(132, 132),    S(107, 113),    //
    S(112, 107),    S(135, 124),    S(156, 135),    S(134, 174),    S(164, 161),    S(101, 146),    S(111, 120),    S(80, 112),     //
    S(131, 80),     S(142, 107),    S(151, 126),    S(160, 148),    S(146, 157),    S(112, 147),    S(75, 128),     S(83, 95),      //
    S(151, 82),     S(186, 88),     S(203, 105),    S(150, 143),    S(142, 129),    S(148, 125),    S(156, 101),    S(95, 107),     //
    S(137, 34),     S(220, 54),     S(179, 69),     S(148, 96),     S(129, 106),    S(138, 77),     S(138, 84),     S(127, 44),     //
    S(140, 45),     S(124, 94),     S(132, 120),    S(150, 76),     S(156, 63),     S(149, 122),    S(144, 77),     S(121, 70),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(404, 242),    S(439, 250),    S(397, 275),    S(412, 256),    S(406, 257),    S(350, 270),    S(367, 271),    S(365, 274),    //
    S(301, 296),    S(361, 287),    S(452, 261),    S(388, 284),    S(389, 286),    S(346, 293),    S(265, 322),    S(265, 320),    //
    S(278, 286),    S(408, 251),    S(442, 243),    S(421, 244),    S(381, 254),    S(333, 278),    S(332, 280),    S(252, 313),    //
    S(255, 274),    S(335, 273),    S(377, 255),    S(350, 256),    S(347, 264),    S(306, 283),    S(284, 285),    S(223, 299),    //
    S(215, 228),    S(289, 237),    S(273, 253),    S(261, 245),    S(252, 254),    S(239, 281),    S(208, 272),    S(180, 271),    //
    S(186, 200),    S(251, 197),    S(254, 220),    S(236, 215),    S(240, 210),    S(213, 246),    S(202, 226),    S(159, 240),    //
    S(103, 215),    S(221, 157),    S(236, 182),    S(241, 187),    S(231, 189),    S(219, 202),    S(196, 186),    S(168, 202),    //
    S(150, 204),    S(163, 226),    S(235, 188),    S(258, 173),    S(239, 192),    S(227, 207),    S(212, 199),    S(193, 222),    //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(466, 400),    S(544, 336),    S(530, 371),    S(447, 477),    S(460, 440),    S(451, 433),    S(488, 364),    S(388, 440),    //
    S(490, 429),    S(432, 533),    S(433, 548),    S(292, 610),    S(296, 604),    S(363, 560),    S(392, 476),    S(415, 452),    //
    S(419, 491),    S(511, 480),    S(440, 549),    S(408, 567),    S(359, 571),    S(375, 525),    S(448, 429),    S(425, 400),    //
    S(493, 391),    S(490, 460),    S(452, 502),    S(426, 555),    S(416, 557),    S(417, 491),    S(470, 398),    S(468, 366),    //
    S(476, 393),    S(488, 411),    S(465, 460),    S(434, 507),    S(437, 503),    S(433, 465),    S(453, 393),    S(464, 344),    //
    S(466, 314),    S(501, 346),    S(502, 413),    S(465, 396),    S(461, 389),    S(468, 410),    S(472, 354),    S(458, 326),    //
    S(450, 189),    S(495, 149),    S(492, 240),    S(503, 285),    S(485, 315),    S(485, 284),    S(455, 333),    S(455, 319),    //
    S(407, 227),    S(456, 31),     S(456, 62),     S(483, 154),    S(490, 229),    S(492, 179),    S(481, 212),    S(432, 262),    //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-478, -214),  S(-21, 90),     S(123, 31),     S(-98, 21),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(134, -108),   S(239, 6),      S(188, 37),     S(134, -23),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(55, 30),      S(261, 18),     S(227, 28),     S(116, 8),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-207, 59),    S(170, 6),      S(55, 30),      S(38, 28),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-227, 45),    S(99, -6),      S(86, 1),       S(-20, 40),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-113, 34),    S(168, -24),    S(103, -0),     S(58, 14),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(4, -11),      S(158, -31),    S(98, -20),     S(28, 5),       S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-150, -11),   S(-8, -8),      S(-98, 3),      S(-100, -26),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(-67, 30), S(-2, -28), S(50, -41), S(27, -30), S(30, -33), S(50, -40), S(-8, -18), }},
  {{ S(-51, 25), S(31, -6), S(57, -47), S(19, -31), S(20, -27), S(55, -42), S(33, 2), }},
  {{ S(-20, 25), S(6, 19), S(28, -38), S(16, -86), S(22, -84), S(35, -35), S(3, 23), }},
  {{ S(-30, 13), S(20, 18), S(27, -1), S(24, 6), S(25, 9), S(19, 7), S(31, 33), }},
}};
// Epoch duration: 8.68854s
  // clang-format on
}  // namespace Clockwork
