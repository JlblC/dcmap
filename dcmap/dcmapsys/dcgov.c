/** @file
 * @brief D&C Governeurs Optimizer
 *
 * Copyright (C) 2006 D&C Governeurs Optimizer by Punch.
 *
 * D&C Governeurs Optimizer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * D&C Governeurs Optimizer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA  02111-1307  USA
 *
 * @author Punch <punch@mail15.com>
 *
 * $Id: $
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define MAX_PLANETS  100    /* Maximum number of planets to take in account */
#define MAX_GOVS     64     /* Maximum number of governeurs that can be used */
#define MAX_CORRUPT  6      /* Size of corruption rule array */

#define MAX_DIST     40     /* Distance the governeurs can affect the planets */
#define MAX_DIST_SQR (MAX_DIST * MAX_DIST)  /* Square of the distance */

#define FALSE   0
#define TRUE    1

#define USE_MEMCPY 1

#define MAX_GOVS_PLACEMENTS             1000
#define OPTIMAL_PLACEMENTS_TO_DUMP      5

#define DCGOV_ITERATIONS_COUNT_DEFAULT      100
#define DCGOV_SLOW_ITERATIONS_COUNT_DEFAULT 100

static int corrupts[] = {0, 10, 25, 50, 85, 95}; /* Corruption rule */
static int incomes[] = {100, 90, 75, 50, 15, 5}; /* Income percentage */

/* Planet Info structure */
typedef struct planet_info_s {
    int x;              /* XXX coord */
    int y;              /* YYY coord */
    int max_corrupt;    /* Maximum acceptable corruption on the planet */
    int full_income;    /* Full income without corruption */

    int corrupt;        /* Corruption Index */
    int income;         /* Income with corruption */
    int gov_no;         /* Index of the Governeur that affects on the planet */

    int neighbs[MAX_PLANETS]; /* Indexes of the planet neighbours */
    int neighbs_count;        /* Amount of neighbours (in R < 40Gbm area) */

    int hw;                   /* whether the planet is HomeWorld or not */
    int mark;                 /* Is the planet affected by any Governeur */
    int safe;                 /* whether the planet is safe enough to land
                                 the governeur */
} planet_info_t;

typedef struct govs_placement_s {
    int income;
    int govs[MAX_GOVS];
} govs_placement_t;

static planet_info_t *planets[MAX_PLANETS]; /* Planets array */
static int planets_count = 0;               /* Total amount of planets */
static int safe_planets_count = 0;          /* Total amount of safe planets */
static int min_income = 0;                  /* Minimal acceptable income */
static int govs_placements_variants = 0;    /* Amount of variants of 
                                             * Governeurs placements to print
                                             */
static long long int total_variants = 0;    /* Total amount of different
                                             * govs placements variants
                                             */
static long long int variants_processed = 0;/* Amount of processed
                                             * govs placements variants
                                             */
static int variant_to_show = -1;            /* Placement Variant number to show */

static int sorted_planets[MAX_PLANETS];     /* Planet indexes sorted by amount of neighbours */

static int limit_max_corruption = TRUE;     /* Use max corruption requirements in total income calculations */

static govs_placement_t *optimal_govs_placements[MAX_GOVS_PLACEMENTS];
static int optimal_govs_placements_count = 0;

/* Short dump of governeurs placements */
static int
dump_short_govs_placement(int govs_count, int *govs_placement)
{
    int gov_no;

    for (gov_no = 0; gov_no < govs_count; gov_no++)
    {
        fprintf(stdout, "%03d:%03d ",
                planets[govs_placement[gov_no]]->x,
                planets[govs_placement[gov_no]]->y);
    }
    fprintf(stdout, "\n");
    
    return 0;
}

/* Dump Possible Governeurs placements */
static int
dump_optimal_govs_placements(int govs_count, int rows_to_dump)
{
    int row_no;
    int gov_no;
    planet_info_t *info;

    fprintf(stdout, "\nPossible Governeurs Placements:\n");
    for (row_no = 0; row_no < rows_to_dump; row_no++)
    {
        if (row_no >= optimal_govs_placements_count)
            break;

        fprintf(stdout, "%2d. %d ", row_no,
                optimal_govs_placements[row_no]->income);
        
        for (gov_no = 0; gov_no < govs_count; gov_no++)
        {
            info = planets[optimal_govs_placements[row_no]->govs[gov_no]];
        
            fprintf(stdout, "%03d:%03d ", info->x, info->y);
        }
        fprintf(stdout, "\n");
    }
    
    return 0;
}

/* Sort Possible Governeurs placements */
static int
sort_optimal_govs_placements(int rows_to_dump)
{
    int row_no, i;
    govs_placement_t *tmp;

    for (row_no = 0; row_no < rows_to_dump; row_no++)
    {
        for (i = row_no + 1; i < optimal_govs_placements_count; i++)
        {
            if (optimal_govs_placements[row_no]->income <
                optimal_govs_placements[i]->income)
            {
                tmp = optimal_govs_placements[row_no];
                optimal_govs_placements[row_no] = optimal_govs_placements[i];
                optimal_govs_placements[i] = tmp;
            }
        }
    }
    
    return 0;
}


/* Dump Governeurs placement */
static int
dump_govs_placement(int govs_count, int *govs_placement)
{
    int gov_no;
    planet_info_t *info;

    fprintf(stdout, "Governeurs Placements Dump\n");
    fprintf(stdout, " N XXX:YYY\n");
    for (gov_no = 0; gov_no < govs_count; gov_no++)
    {
        info = planets[govs_placement[gov_no]];
        
        fprintf(stdout, "%2d %03d:%03d\n", gov_no, info->x, info->y);
    }
    
    return 0;
}

/* Dump Planets Info */
static int
dump_planets_info(int *govs_placement)
{
    int planet_no;
    planet_info_t *info;
    
    fprintf(stdout, "Planets Info Dump\n");
    fprintf(stdout, "XXX:YYY Income (Max) Corruption(Max) Governeur (XXX:YYY) Neighbours\n");
    for (planet_no = 0; planet_no < planets_count; planet_no++)
    {
        info = planets[planet_no];
        
        fprintf(stdout, "%03d:%03d %5d(%5d)        %3d(%3d)        %2d (%3d:%3d)     %3d    %s %s\n",
                info->x, info->y, info->income, info->full_income,
                corrupts[info->corrupt], info->max_corrupt, info->gov_no,
                (info->gov_no >= 0) ? planets[govs_placement[info->gov_no]]->x : -1,
                (info->gov_no >= 0) ? planets[govs_placement[info->gov_no]]->y : -1,
                info->neighbs_count - 1,
                (info->safe == TRUE) ? "Safe" : "Unsafe",
                (info->hw == TRUE)? "HW" : "");
    }
    
    return 0;
}

/* Dump Planets Neighbours */
static int
dump_planets_distances()
{
    int i;
    int planet_no;
    planet_info_t *info;
    
    fprintf(stdout, "Planets Distances\n");
    fprintf(stdout, "XXX:YYY NeighboursCount: <Neighbours>\n");
    for (planet_no = 0; planet_no < planets_count; planet_no++)
    {
        info = planets[planet_no];
        
        fprintf(stdout, "%03d:%03d %2d: ", info->x, info->y, info->neighbs_count - 1);
        for (i = 1; i < info->neighbs_count; i++)
            fprintf(stdout, "%03d:%03d ", planets[info->neighbs[i]]->x, planets[info->neighbs[i]]->y);
        fprintf(stdout, "\n");
    }
    
    return 0;
}

/* Calculate Total Income with Governeurs placement */
static int
calc_total_income(int govs_count, int *gov_places)
{
    int i;
    int corrupt;
    int gov;
    
    int income = 0;
    int planets_handled[MAX_GOVS];
    planet_info_t *info = NULL;
    int handled = 0;

    //fprintf(stdout, "\ncalc_total_income(started)\n");
    //dump_govs_placement(govs_count, gov_places);
    
    memset((void *)&planets_handled[0], 0, sizeof(int) * MAX_GOVS);

    for (i = 0; i < planets_count; i++)
    {
        planets[i]->mark = FALSE;
        planets[i]->gov_no = -1;
    }

    /* Calculate Corruption on planets affected by the gouverneurs */
    for (corrupt = 0; corrupt < MAX_CORRUPT - 1; corrupt++)
    {
        for (gov = 0; gov < govs_count; gov++)
        {
            info = planets[gov_places[gov]];
            handled = planets_handled[gov];

            while ((handled < info->neighbs_count) &&
                   (planets[info->neighbs[handled]]->mark == TRUE))
                handled++;

            if (handled < info->neighbs_count)
            {
                planets[info->neighbs[handled]]->mark = TRUE;
                planets[info->neighbs[handled]]->corrupt = corrupt;
                planets[info->neighbs[handled]]->gov_no = gov;

                /* Verify that calculated corruption does apply to curruption
                 * requirement, return 0 income in case this verification is
                 * failed
                 */
                if ((limit_max_corruption == TRUE) &&
                    (planets[info->neighbs[handled]]->max_corrupt < corrupts[corrupt]))
                    return 0;
            }

            planets_handled[gov] = handled;
        }
    }

    /* Calculate total income */
    for (i = 0; i < planets_count; i++)
    {
        if (planets[i]->mark == TRUE)
            planets[i]->income = planets[i]->full_income * incomes[planets[i]->corrupt];
        else
        {
            if (planets[i]->hw == TRUE)
            {
                planets[i]->income = planets[i]->full_income * 100;
            }
            else if ((limit_max_corruption == FALSE) ||
                     (planets[i]->max_corrupt >= corrupts[MAX_CORRUPT - 1]))
            {
                planets[i]->corrupt = MAX_CORRUPT - 1;
                planets[i]->income =
                    planets[i]->full_income * incomes[MAX_CORRUPT - 1];
            }
            else
            {
                /* Corruption level does not confirm the requirements */
                return 0;
            }
        }

        income += planets[i]->income;

        planets[i]->income = (planets[i]->income + 50) / 100; /* Make division and round here */
    }

    return (income + 50) / 100; /* Return here total rounded income */
}

/* Load Planets Info from the file */
static int
load_planets(char *filename)
{
    FILE *src;
    planet_info_t info;
    int line = 0;
    int rc = 0;
    char safe;

    if ((src = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Cannot open file %s for input, errno=%d\n",
                filename, errno);
        return -ENOENT;
    }

    while (!feof(src))
    {
        line++;
        memset(&info, 0, sizeof(planet_info_t));
        rc = fscanf(src, "%d:%d;%d;%d", &info.x, &info.y,
                    &info.full_income, &info.max_corrupt);
        if (rc == 3)
            info.max_corrupt = 100;
        else if (rc < 3)
        {
            fclose(src);
            if (rc <= 0)
            {
                fprintf(stdout, "%d planets parsed\n", line - 1);
                return 0;
            }
            else
            {
                fprintf(stderr, "Wrong input file format, line %d, rc=%d\n", line, rc);
                return -EINVAL;
            }
        }
        if (info.max_corrupt < 0)
            info.max_corrupt = 100;

        if ((fscanf(src, "%c", &safe) == 1) && (safe == '!'))
            info.safe = FALSE;
        else
        {
            info.safe = TRUE;
            safe_planets_count++;
        }

        /* Mark the first planet in list as HW */
        info.hw = (line == 1) ? TRUE : FALSE;

        planets[planets_count] = (planet_info_t *)malloc(sizeof(planet_info_t));
        *planets[planets_count++] = info;
    }

    fclose(src);

    return 0;
}

/* Load Governeurs Placements from the file */
static int
load_govs_placement(char *filename, int *govs_placement, int *govs_count)
{
    FILE *src;
    planet_info_t info;
    int line = 0;
    int rc = 0;
    int planet_no;

    *govs_count = 0;

    if ((src = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Cannot open file %s for input, errno=%d\n",
                filename, errno);
        return -ENOENT;
    }

    while (!feof(src))
    {
        line++;
        memset(&info, 0, sizeof(planet_info_t));
        rc = fscanf(src, "%d:%d", &info.x, &info.y);
        if (rc < 2)
        {
            fclose(src);
            if (rc <= 0)
            {
                fprintf(stdout, "%d governeurs parsed\n", line - 1);
                return 0;
            }
            else
            {
                fprintf(stderr, "Wrong input file format, line %d, rc=%d\n", line, rc);
                rc = -EINVAL;
                break;
            }
        }

        /* Mark the first planet in list as HW */
        for (planet_no = 0; planet_no < planets_count; planet_no++)
        {
            if ((planets[planet_no]->x == info.x) &&
                (planets[planet_no]->y == info.y))
            {
                govs_placement[(*govs_count)++] = planet_no;
                rc = 0;
                break;
            }
        }
        if (planet_no >= planets_count)
        {
            fprintf(stderr, "Invalid Govreneur location %03d:%03d, line %d\n",
                    info.x, info.y, line);
            rc = -EINVAL;
            break;
        }

        if (*govs_count >= MAX_GOVS)
        {
            fprintf(stderr, "Maximum number of govreneurs (%d) exceeded\n",
                    MAX_GOVS);
            rc = 0;
            break;
        }
    }

    fclose(src);

    return rc;
}

/* Calculate distances between the planets and sort planet neighbours */
static int
calc_planet_distances()
{
    int i, j, k, tmp, dx, dy;
    int dist[MAX_PLANETS];
    
    memset(dist, 0, sizeof(dist));

    for (i = 0; i < planets_count; i++)
    {
        /* Calculate sqare of distance */
        for (j = 0; j < planets_count - 1; j++)
        {
            dx = planets[i]->x - planets[j]->x;
            dy = planets[i]->y - planets[j]->y;
            dist[j] = dx * dx + dy * dy;
            planets[i]->neighbs[j] = j;
        }
        
        /* Sort planet neighbours by distance */
        for (j = 0; j < planets_count - 1; j++)
        {
            for (k = j + 1; k < planets_count - 1; k++)
            {
                if (dist[j] > dist[k])
                {
                    tmp = dist[j];
                    dist[j] = dist[k];
                    dist[k] = tmp;
                    
                    tmp = planets[i]->neighbs[j];
                    planets[i]->neighbs[j] = planets[i]->neighbs[k];
                    planets[i]->neighbs[k] = tmp;
                }
            }
        }

        for (j = 0; j < planets_count - 1; j++)
        {
            if (dist[j] < MAX_DIST_SQR)
                planets[i]->neighbs_count++;
        }
    }

    return 0;
}

/* Sort planets by the coordinates to correspond the original D&C order */
static int
sort_planets_by_coords()
{
    int i, j;
    planet_info_t   *tmp;

    /* Move HW to the end of list */
    tmp = planets[0];
    planets[0] = planets[planets_count - 1];
    planets[planets_count - 1] = tmp;
    
    for (i = 0; i < planets_count - 2; i++)
    {
        for (j = i + 1; j < planets_count - 1; j++)
        {
            if ((planets[i]->y > planets[j]->y) ||
                ((planets[i]->y == planets[j]->y) &&
                 (planets[i]->x > planets[j]->x)))
            {
                tmp = planets[i];
                planets[i] = planets[j];
                planets[j] = tmp;
            }
        }
    }
    
    return 0;
}

/* Recursive governeurs optimisation implementation */
static int
find_optimized_govs_placement(int govs_used, int govs_count,
                              int planet_to_occupy_first,
                              int *govs_placement)
{
#if !USE_MEMCPY
    int k;
#endif
    int planet_no;
    int optimal_govs_placement[MAX_GOVS];
    int max_income = 0;
    int income = 0;


    for (planet_no = planet_to_occupy_first; planet_no < planets_count; planet_no++)
    {
        if (planets[sorted_planets[planet_no]]->safe == FALSE)
            continue;

        govs_placement[govs_used] = sorted_planets[planet_no];

        if (govs_used + 1 < govs_count)
        {
            income = find_optimized_govs_placement(govs_used + 1, govs_count,
                                                   planet_no, govs_placement);
        }
        else
        {
            income = calc_total_income(govs_count, govs_placement);
            if ((min_income > 0) && (income >= min_income) &&
                (optimal_govs_placements_count < MAX_GOVS_PLACEMENTS))
            {
                optimal_govs_placements[optimal_govs_placements_count] =
                    (govs_placement_t *)malloc(sizeof(govs_placement_t));
                optimal_govs_placements[optimal_govs_placements_count]->income = income;
                memcpy(optimal_govs_placements[optimal_govs_placements_count]->govs,
                       govs_placement, sizeof(int) * govs_count);
                optimal_govs_placements_count++;
            }
            variants_processed++;
        }

        if (income > max_income)
        {
            /* Store optimal placement in temporary array */
#if USE_MEMCPY
            memcpy(optimal_govs_placement, govs_placement,
                   sizeof(int) * govs_count);
#else
            for (k = govs_used; k < govs_count; k++)
                optimal_govs_placement[k] = govs_placement[k];
#endif

            max_income = income;
        }
        
        if (govs_used <= 3)
        {
            fprintf(stdout, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b%lld%% done.",
                    (variants_processed * 100) / total_variants);
            fprintf(stdout, ".");
            fflush(stdout);
        }
    }
    if (govs_used == 0)
        fprintf(stdout, "\n");
    
    if (max_income > 0)
    {
        /* Restore optimal placement */
#if USE_MEMCPY
        memcpy(govs_placement, optimal_govs_placement,
               sizeof(int) * govs_count);
#else
        for (k = govs_used; k < govs_count; k++)
            govs_placement[k] = optimal_govs_placement[k];
#endif
    }

    return max_income;
}

/* Sort planets by amount of neighbours to speedup calculation of corruption */
static int
sort_planets_by_neighbs_count()
{
    int i, j, tmp;
    
    for (i = 0; i < planets_count; i++)
        sorted_planets[i] = i;

    /* Sort planets by the amount of neighbours */ 
    for (i = 0; i < planets_count; i++)
    {
        for (j = i + 1; j < planets_count; j++)
        {
            if (planets[sorted_planets[i]]->neighbs_count >
                planets[sorted_planets[j]]->neighbs_count)
            {
                tmp = sorted_planets[i];
                sorted_planets[i] = sorted_planets[j];
                sorted_planets[j] = tmp;
            }
        }
    }
    
    return 0;
}

/* Sort governeurs by the number of planets they can affect */
static int
sort_govs(int govs_count, int *govs_placement)
{
    int i, j, tmp;
    
    for (i = 0; i < govs_count - 1; i++)
    {
        for (j = i + 1; j < govs_count; j++)
        {
            if (planets[govs_placement[i]]->neighbs_count >
                planets[govs_placement[j]]->neighbs_count)
            {
                tmp = govs_placement[i];
                govs_placement[i] = govs_placement[j];
                govs_placement[j] = tmp;
            }
        }
    }
    return 0;
}

/*
 * N-1 governeurs are already sorted, one should fine the right place
 * for the last governeur
 */
static int
insert_sorted_govs(int govs_count, int *govs_placement, int gov_to_insert)
{
    int i, tmp;
    
    for (i = govs_count - 1; i > 0; i--)
    {
        while (planets[govs_placement[i - 1]]->neighbs_count >
               planets[govs_placement[i]]->neighbs_count)
        {
            tmp = govs_placement[i];
            govs_placement[i] = govs_placement[i - 1];
            govs_placement[i - 1] = tmp;
        }
    }
}

static long long int
power(int n, int p)
{
    int i;
    long long int result = 1;
    
    for (i = 0; i < p; i++)
        result *= n;

    return result;
}

static long long int
factorial(int n)
{
    int i;
    long long int result = 1;
    
    for (i = 2; i <= n; i++)
        result *= i;

    return result;
}

static long long int
calc_total_variants(int govs_count, int safe_planets)
{
    int i;
    long long int result = 0;

    if (govs_count <= 0)
        return 1;

    if (govs_count == 1)
        return safe_planets;

    for (i = 0; i < safe_planets; i++)
    {
        if (govs_count > 2)
            result += calc_total_variants(govs_count - 1, safe_planets - i);
        else
            result += safe_planets - i;
    }

    return result;
}

/* Recursive governeurs optimisation implementation */
static int
optimize_govs_placement_full(int govs_count)
{
    int max_income = 0;
    
    int govs_placement[MAX_GOVS];

    variants_processed = 0;
    /* Very roughly approximation */
    total_variants = power(safe_planets_count, govs_count) / factorial(govs_count);

    fprintf(stdout, "\nOptimization of %d governeurs between %d planets, ~%lld variants, "
                    "please wait...\n", govs_count, planets_count, total_variants);

    total_variants = calc_total_variants(govs_count, safe_planets_count);

    fprintf(stdout, "\nOptimization of %d governeurs between %d planets, ~%lld variants, "
                    "please wait...\n", govs_count, planets_count, total_variants);

    /* Find optimal placement */
    memset(govs_placement, 0, sizeof(int) * MAX_GOVS);
    max_income = find_optimized_govs_placement(0, govs_count, 0,
                                               govs_placement);
    if (max_income <= 0)
    {
        fprintf(stderr, "\nInvalid governeurs placement requirements.\n"
                "Try softer requirements for maximal corruption or "
                "higher amount of governeurs\n");
    }

    /* Output calculation here */
    max_income = calc_total_income(govs_count, govs_placement);

    fprintf(stdout, "\nOptimal Corruption on Planets:\n");
    dump_planets_info(govs_placement);

    fprintf(stdout, "\nOptimal Governeurs Placement:\n");
    dump_govs_placement(govs_count, govs_placement);

    fprintf(stdout, "\nMaximum Income: %d Credits\n", max_income);

    return 0;    
}


static int
enhance_step(int govs_count, int *govs_placement, int *enhanced_gov, int *enhanced_planet)
{
    int gov_no;
    int planet_no;
    int orig_income;
    int tmp_income;
    int max_income;
    int tmp_govs_placement[MAX_GOVS];
    
    *enhanced_gov = -1;
    *enhanced_planet = -1;

    orig_income = calc_total_income(govs_count, govs_placement);

    max_income = orig_income;
    for (gov_no = 0; gov_no < govs_count; gov_no++)
    {
        for (planet_no = 0; planet_no < planets_count; planet_no++)
        {
            if (planets[planet_no]->safe == FALSE)
                continue;

            memcpy(tmp_govs_placement, govs_placement, 
                   sizeof(int) * govs_count);
            tmp_govs_placement[gov_no] = planet_no;
            sort_govs(govs_count, tmp_govs_placement);
            tmp_income = calc_total_income(govs_count, tmp_govs_placement);

            if (tmp_income > max_income)
            {
                *enhanced_gov = gov_no;
                *enhanced_planet = planet_no;
                max_income = tmp_income;
            }
        }
    }

    return max_income;
}


static int
deep_enhance_step(int govs_count, int *govs_placement,
                  int *enhanced_gov1, int *enhanced_planet1,
                  int *enhanced_gov2, int *enhanced_planet2)
{
    int gov1_no;
    int gov2_no;
    int planet1_no;
    int planet2_no;
    int orig_income;
    int tmp_income;
    int max_income;
    int tmp_govs_placement[MAX_GOVS];
    
    *enhanced_gov1 = -1;
    *enhanced_planet1 = -1;

    *enhanced_gov2 = -1;
    *enhanced_planet2 = -1;

    orig_income = calc_total_income(govs_count, govs_placement);

    max_income = orig_income;
    for (gov1_no = 0; gov1_no < govs_count; gov1_no++)
    {
        for (gov2_no = gov1_no + 1; gov2_no < govs_count; gov2_no++)
        {
            for (planet1_no = 0; planet1_no < planets_count; planet1_no++)
            {
                if (planets[planet1_no]->safe == FALSE)
                    continue;

                for (planet2_no = 0; planet2_no < planets_count; planet2_no++)
                {
                    if (planets[planet2_no]->safe == FALSE)
                        continue;

                    memcpy(tmp_govs_placement, govs_placement, 
                           sizeof(int) * govs_count);

                    tmp_govs_placement[gov1_no] = planet1_no;
                    tmp_govs_placement[gov2_no] = planet2_no;
                    sort_govs(govs_count, tmp_govs_placement);
                    tmp_income = calc_total_income(govs_count, tmp_govs_placement);

                    if (tmp_income > max_income)
                    {
                        *enhanced_gov1 = gov1_no;
                        *enhanced_planet1 = planet1_no;
                        *enhanced_gov2 = gov2_no;
                        *enhanced_planet2 = planet2_no;
                        max_income = tmp_income;
                    }
                }
            }
        }
    }

    return max_income;
}

static int
liftup_gov(int govs_count, int *govs_placement, int gov_to_liftup)
{
    int income;
    int tmp_govs_placement[MAX_GOVS];
    
    memcpy(tmp_govs_placement, govs_placement,
           sizeof(int) * gov_to_liftup);
    memcpy(tmp_govs_placement + gov_to_liftup,
           govs_placement + gov_to_liftup + 1,
           sizeof(int) * (govs_count - gov_to_liftup - 1));

    /* Turn off the max curruption limits */
    limit_max_corruption = FALSE;
    income = calc_total_income(govs_count - 1, tmp_govs_placement);
    limit_max_corruption = TRUE;

    return income;
}

static int
enhance_govs_placement(int govs_count, int *govs_placement, int enhanced_steps)
{
    int orig_income;
    int max_income;
    int middle_income;
    int enhanced_gov;
    int enhanced_planet;
    int step_no;

    fprintf(stdout, "Start enhancing governeurs placement, "
            "%d enhancment to proceed\n", enhanced_steps);
    
    dump_short_govs_placement(govs_count, govs_placement);
    for (step_no = 0; step_no < enhanced_steps; step_no++)
    {
        orig_income = calc_total_income(govs_count, govs_placement);
        max_income = enhance_step(govs_count, govs_placement, &enhanced_gov, &enhanced_planet);
        if (max_income <= orig_income)
        {
            fprintf(stdout, "Cannot find enhancments any more, "
                    "only %d enhancments done", step_no);
            break;
        }
        middle_income = liftup_gov(govs_count, govs_placement, enhanced_gov);

        fprintf(stdout, "%3d. Governeur movement: %03d:%03d --> %03d:%03d, "
                "Income growth: %d --> (%d) --> %d\n", step_no + 1,
                planets[govs_placement[enhanced_gov]]->x,
                planets[govs_placement[enhanced_gov]]->y,
                planets[enhanced_planet]->x,
                planets[enhanced_planet]->y,
                orig_income, middle_income, max_income);
        govs_placement[enhanced_gov] = enhanced_planet;
        sort_govs(govs_count, govs_placement);
        dump_short_govs_placement(govs_count, govs_placement);
    }

    /* Output calculation here */
    max_income = calc_total_income(govs_count, govs_placement);

    fprintf(stdout, "\nEnhanced Corruption on Planets:\n");
    dump_planets_info(govs_placement);

    fprintf(stdout, "\nEnhanced Governeurs Placement:\n");
    dump_govs_placement(govs_count, govs_placement);

    fprintf(stdout, "\nEnhanced Income: %d Credits\n", max_income);
    
    return 0;
}


static int
deep_enhance_govs_placement(int govs_count, int *govs_placement, int enhanced_steps)
{
    int orig_income;
    int max_income;
    int enhanced_gov1;
    int enhanced_planet1;
    int enhanced_gov2;
    int enhanced_planet2;
    int step_no;

    fprintf(stdout, "Start enhancing governeurs placement, "
            "%d enhancment to proceed\n", enhanced_steps);
    
    dump_short_govs_placement(govs_count, govs_placement);
    for (step_no = 0; step_no < enhanced_steps; step_no++)
    {
        orig_income = calc_total_income(govs_count, govs_placement);
        max_income = deep_enhance_step(govs_count, govs_placement,
                                       &enhanced_gov1, &enhanced_planet1,
                                       &enhanced_gov2, &enhanced_planet2);
        if (max_income <= orig_income)
        {
            fprintf(stdout, "Cannot find enhancments any more, "
                    "only %d enhancments done", step_no);
            break;
        }

        fprintf(stdout, "%3d. Governeurs movement: %03d:%03d --> %03d:%03d and %03d:%03d --> %03d:%03d, "
                "Income growth: %d --> %d\n", step_no + 1,
                planets[govs_placement[enhanced_gov1]]->x,
                planets[govs_placement[enhanced_gov1]]->y,
                planets[enhanced_planet1]->x,
                planets[enhanced_planet1]->y,
                planets[govs_placement[enhanced_gov2]]->x,
                planets[govs_placement[enhanced_gov2]]->y,
                planets[enhanced_planet2]->x,
                planets[enhanced_planet2]->y,
                orig_income, max_income);
        govs_placement[enhanced_gov1] = enhanced_planet1;
        govs_placement[enhanced_gov2] = enhanced_planet2;
        sort_govs(govs_count, govs_placement);
        dump_short_govs_placement(govs_count, govs_placement);
    }

    /* Output calculation here */
    max_income = calc_total_income(govs_count, govs_placement);

    fprintf(stdout, "\nEnhanced Corruption on Planets:\n");
    dump_planets_info(govs_placement);

    fprintf(stdout, "\nEnhanced Governeurs Placement:\n");
    dump_govs_placement(govs_count, govs_placement);

    fprintf(stdout, "\nEnhanced Income: %d Credits\n", max_income);
    
    return 0;
}


static int
generate_random_govs_placements(int govs_count, int *govs_placements)
{
    int gov_no;
    for (gov_no = 0; gov_no < govs_count; gov_no++)
    {
        do {
            govs_placements[gov_no] = rand() % planets_count;
        }
        while (planets[govs_placements[gov_no]]->safe != TRUE);
    }
    sort_govs(govs_count, govs_placements);
    
    return calc_total_income(govs_count, govs_placements);
}

/* Iterative governeurs optimisation implementation */
static int
optimize_govs_placement_iterate(int govs_count, int iterations_count,
                                int slow_iterations)
{
    int income;
    int orig_income;
    int max_income = 0;

    int enhanced_gov;
    int enhanced_planet;

    int enhanced_gov2;
    int enhanced_planet2;

    int step_no;
    int iter_no;
    int progress;
    
    int govs_placement[MAX_GOVS];
    int optimal_govs_placement[MAX_GOVS];

    fprintf(stdout, "\nOptimization of %d governeurs between %d planets, "
                    "please wait...\n", govs_count, planets_count);

    max_income = 0;
    progress = 0;
    fprintf(stdout, "%d%% iterations done. Optimal income %d credits",
                    progress, max_income);
    fflush(stdout);

    for (iter_no = 0; iter_no < iterations_count; iter_no++)
    {
        income =
            generate_random_govs_placements(govs_count, govs_placement);

#if 0
            dump_short_govs_placement(govs_count, govs_placement);
#endif

        for (step_no = 0;; step_no++)
        {
            orig_income = income;

            if (slow_iterations == TRUE)
            {
                income = deep_enhance_step(govs_count, govs_placement,
                                           &enhanced_gov, &enhanced_planet,
                                           &enhanced_gov2, &enhanced_planet2);
            }
            else
            {
                income = enhance_step(govs_count, govs_placement,
                                      &enhanced_gov, &enhanced_planet);
            }

            if (income <= orig_income)
            {
#if 0
                fprintf(stdout, "Cannot find enhancments any more, "
                        "only %d enhancments done", step_no);
#endif
                break;
            }

            govs_placement[enhanced_gov] = enhanced_planet;

            if (slow_iterations == TRUE)
                govs_placement[enhanced_gov2] = enhanced_planet2;

            sort_govs(govs_count, govs_placement);

#if 0
            dump_short_govs_placement(govs_count, govs_placement);
#endif
        }
        
        if (max_income < income)
        {
            memcpy(optimal_govs_placement, govs_placement,
                   sizeof(int) * govs_count);
            max_income = calc_total_income(govs_count, optimal_govs_placement);

            fprintf(stdout, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
                            "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
                            "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
                            "%d-th iteration. %d Credits, Govs: ", iter_no, max_income);
            dump_short_govs_placement(govs_count, govs_placement);
        }

        if (((iter_no + 1) * 100 / iterations_count) != progress)
        {
            progress = ((iter_no + 1) * 100 / iterations_count);
            
            fprintf(stdout, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
                            "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
                            "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
                            "%d%% iterations done. Optimal income %d credits",
                            progress, max_income);
            fflush(stdout);
        }
    }
    fprintf(stdout, "\n");

    /* Output calculation here */
    memcpy(govs_placement, optimal_govs_placement, sizeof(int) * govs_count);
    max_income = calc_total_income(govs_count, govs_placement);
    
    fprintf(stdout, "\nOptimal Corruption on Planets:\n");
    dump_planets_info(govs_placement);

    fprintf(stdout, "\nOptimal Governeurs Placement:\n");
    dump_govs_placement(govs_count, govs_placement);

    fprintf(stdout, "\nMaximum Income: %d Credits\n", max_income);

    return 0;    
}

static char *syntax_error_msg =
    "Invalid syntax usage.\n"
    "  Use: dcgov [-f|--full] <input-file> <governeurs-number> [<minimal_income> [<best_variants_number> [<variant_to_show>l]]]\n"
    "       dcgov <-i|--iterate> <input-file> <governeurs-number> [number_of_iterations]\n"
    "       dcgov <-o|--slow-iterate> <input-file> <governeurs-number> [number_of_iterations]\n"
    "       dcgov <-e|--enhance> <input-file> <governeurs-placement-file> <enhancements_count>\n"
    "       dcgov <-d|--deep-enhance> <input-file> <governeurs-placement-file> <enhancements_count>\n"
    "       dcgov <-s|--show> <input-file> <governeurs-placement-file>\n"
    "       dcgov <-u|--up> <input-file> <governeurs-placement-file>\n"
    "       dcgov <-h|--help>\n";

enum {
    DCGOV_OPTIMIZE_METHOD_INVALID       = 0,
    DCGOV_OPTIMIZE_METHOD_FULL          = 1,
    DCGOV_OPTIMIZE_METHOD_ITERATE       = 2,
    DCGOV_OPTIMIZE_METHOD_ENHANCE       = 3,
    DCGOV_OPTIMIZE_METHOD_SHOW          = 4,
    DCGOV_OPTIMIZE_METHOD_LIFTUP        = 5,
    DCGOV_OPTIMIZE_METHOD_DEEP_ENHANCE  = 6,
    DCGOV_OPTIMIZE_METHOD_SLOW_ITERATE  = 7,
};

int
main(int argc, char *argv[])
{
    int rc;
    int govs = 0;
    time_t start = 0;
    time_t finish = 0;
    char *planets_filename = NULL;
    char *govs_filename = NULL;
    char *optimize_method_str = NULL;
    int   optimize_method = DCGOV_OPTIMIZE_METHOD_INVALID;
    int   orig_govs_placement[MAX_GOVS];
    int   enhanced_steps = 1;
    int   iterations_count;

    srand(time(NULL));

    start = time(NULL);

    if (argc < 2)
    {
        fprintf(stderr, syntax_error_msg);
        return -1;
    }

    optimize_method_str = argv[1];
    if ((strcmp(optimize_method_str, "-f") == 0) ||
        (strcmp(optimize_method_str, "--full") == 0))
    {
        optimize_method = DCGOV_OPTIMIZE_METHOD_FULL;
    }
    else
    if ((strcmp(optimize_method_str, "-i") == 0) ||
        (strcmp(optimize_method_str, "--iterate") == 0))
    {
        optimize_method = DCGOV_OPTIMIZE_METHOD_ITERATE;
    }
    else
    if ((strcmp(optimize_method_str, "-o") == 0) ||
        (strcmp(optimize_method_str, "--slow-iterate") == 0))
    {
        optimize_method = DCGOV_OPTIMIZE_METHOD_SLOW_ITERATE;
    }
    else
    if ((strcmp(optimize_method_str, "-e") == 0) ||
        (strcmp(optimize_method_str, "--enhance") == 0))
    {
        optimize_method = DCGOV_OPTIMIZE_METHOD_ENHANCE;
    }
    else
    if ((strcmp(optimize_method_str, "-d") == 0) ||
        (strcmp(optimize_method_str, "--deep-enhance") == 0))
    {
        optimize_method = DCGOV_OPTIMIZE_METHOD_DEEP_ENHANCE;
    }
    else
    if ((strcmp(optimize_method_str, "-s") == 0) ||
        (strcmp(optimize_method_str, "--show") == 0))
    {
        optimize_method = DCGOV_OPTIMIZE_METHOD_SHOW;
    }
    else
    if ((strcmp(optimize_method_str, "-u") == 0) ||
        (strcmp(optimize_method_str, "--up") == 0))
    {
        optimize_method = DCGOV_OPTIMIZE_METHOD_LIFTUP;
    }
    else
    if ((strcmp(optimize_method_str, "-h") == 0) ||
        (strcmp(optimize_method_str, "--help") == 0))
    {
        fprintf(stderr, syntax_error_msg);
        return 0;
    }
    else
    {
        optimize_method = DCGOV_OPTIMIZE_METHOD_FULL;
        argc++;
        argv--;
    }

    argc--;
    argv++;
    
    if (argc < 3)
    {
        fprintf(stderr, syntax_error_msg);
        return -1;
    }
    
    planets_filename = argv[1];

    printf("\nLoad planets info from file %s\n", argv[1]);
    if ((rc = load_planets(planets_filename)) != 0)
    {
        return rc;
    }

    printf("\nSort Planets by Coords\n");
    sort_planets_by_coords();

    printf("\nCalculate Distances and Neighbours\n");
    calc_planet_distances();
    //dump_planets_distances();

    printf("\nSort Planets by Neighbours Count\n");
    sort_planets_by_neighbs_count();

    switch (optimize_method)
    {
        case DCGOV_OPTIMIZE_METHOD_FULL:
        {
            govs = atoi(argv[2]);
            printf("\nOptimize planets for %d governeurs\n", govs);

            if (argc >= 4)
            {
                min_income = atoi(argv[3]);
            }

            if (argc >= 5)
            {
                govs_placements_variants = atoi(argv[4]);
            }
            if (govs_placements_variants <= 0)
            {
                govs_placements_variants = OPTIMAL_PLACEMENTS_TO_DUMP;
            }

            if (argc >= 6)
            {
                variant_to_show = atoi(argv[5]);
            }
    
            //printf("\nOptimize Governeurs Placement");
            optimize_govs_placement_full(govs);

            if (min_income > 0)
            {
                printf("\nOther optimal governeurs placements:\n");
                sort_optimal_govs_placements(govs_placements_variants);
                dump_optimal_govs_placements(govs, govs_placements_variants);
            }

            if (variant_to_show >= 0)
            {
                int income = calc_total_income(govs, optimal_govs_placements[variant_to_show]->govs);

                fprintf(stdout, "\nSelected variant N%d\n", variant_to_show);
                fprintf(stdout, "\nCorruption on Planets:\n");
                dump_planets_info(optimal_govs_placements[variant_to_show]->govs);
                fprintf(stdout, "\nGoverneurs Placement:\n");
                dump_govs_placement(govs, optimal_govs_placements[variant_to_show]->govs);
                fprintf(stdout, "\nIncome: %d Credits\n", income);
            }
        }
        break;

        case DCGOV_OPTIMIZE_METHOD_ITERATE:
        case DCGOV_OPTIMIZE_METHOD_SLOW_ITERATE:
        {
            govs = atoi(argv[2]);
            printf("\nOptimize planets for %d governeurs\n", govs);

            if (argc >= 4)
            {
                iterations_count = atoi(argv[3]);
            }
            else
            {
                if (optimize_method == DCGOV_OPTIMIZE_METHOD_SLOW_ITERATE)
                    iterations_count = DCGOV_SLOW_ITERATIONS_COUNT_DEFAULT * govs;
                else
                    iterations_count = DCGOV_ITERATIONS_COUNT_DEFAULT * govs;
            }

            optimize_govs_placement_iterate(govs, iterations_count,
                (optimize_method == DCGOV_OPTIMIZE_METHOD_SLOW_ITERATE) ?
                TRUE : FALSE);
        }
        break;

        case DCGOV_OPTIMIZE_METHOD_SHOW:
        case DCGOV_OPTIMIZE_METHOD_LIFTUP:
        case DCGOV_OPTIMIZE_METHOD_ENHANCE:
        case DCGOV_OPTIMIZE_METHOD_DEEP_ENHANCE:
        {
            govs_filename = argv[2];
            
            printf("\nLoad governeurs info from file %s\n", govs_filename);
            if ((rc = load_govs_placement(govs_filename, orig_govs_placement,
                                          &govs)) != 0)
            {
                return rc;
            }

            sort_govs(govs, orig_govs_placement);            
            //dump_govs_placement(govs, orig_govs_placement);
            
            if (optimize_method == DCGOV_OPTIMIZE_METHOD_SHOW)
            {
                int income = calc_total_income(govs, orig_govs_placement);

                fprintf(stdout, "\nCorruption on Planets:\n");
                dump_planets_info(orig_govs_placement);

                fprintf(stdout, "\nGoverneurs Placement:\n");
                dump_govs_placement(govs, orig_govs_placement);

                fprintf(stdout, "\nIncome: %d Credits\n", income);
            }
            else
            if ((optimize_method == DCGOV_OPTIMIZE_METHOD_ENHANCE) ||
                (optimize_method == DCGOV_OPTIMIZE_METHOD_DEEP_ENHANCE))
            {
                if (argc >= 4)
                    enhanced_steps = atoi(argv[3]);
                else
                    enhanced_steps = 1;

                if (optimize_method == DCGOV_OPTIMIZE_METHOD_ENHANCE)
                    enhance_govs_placement(govs, orig_govs_placement, enhanced_steps);
                else
                    deep_enhance_govs_placement(govs, orig_govs_placement, enhanced_steps);
            }
            else
            if (optimize_method == DCGOV_OPTIMIZE_METHOD_LIFTUP)
            {
                int gov_no;
                int reduced_income;
                int income = calc_total_income(govs, orig_govs_placement);

                fprintf(stdout, "\nGoverneurs Placement:\n");
                dump_govs_placement(govs, orig_govs_placement);

                fprintf(stdout, "\nIncome: %d Credits\n", income);

                for (gov_no = 0; gov_no < govs; gov_no++)
                {
                    reduced_income = liftup_gov(govs, orig_govs_placement, gov_no);
                    fprintf(stdout, "%2d. Lift up (%03d:%03d) --> %d credits\n", gov_no,
                                    planets[orig_govs_placement[gov_no]]->x,
                                    planets[orig_govs_placement[gov_no]]->y,
                                    reduced_income);
                }
            }
            else
            {
                /* This should never happen */
                return -1;
            }
        }
    }

    finish = time(NULL);
    
    fprintf(stdout, "\n%d seconds for optimising of %d governeurs on %d planets\n", 
            (int)(finish - start), govs, planets_count);
    
    return 0;
}
