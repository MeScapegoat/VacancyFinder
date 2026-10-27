#include <gtest/gtest.h>

#include <QFile>
#include <QDir>

#include <LinkedInParserStrategy/LinkedInParserStrategy.h>


TEST(LinkedInParserStrategy, ClassifySummaryPage)
{
    QFile file(":/full_summaries.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QString html = file.readAll();

    const LinkedInParserStrategy strategy;
    const PageType pageType = strategy.classifyPage(html);

    EXPECT_EQ(pageType, PageType::Summary);
}

TEST(LinkedInParserStrategy, ClassifyHtmlDetailsPage)
{
    QFile file(":/html_1.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QString html = file.readAll();

    const LinkedInParserStrategy strategy;
    const PageType pageType = strategy.classifyPage(html);

    EXPECT_EQ(pageType, PageType::Details);
}

TEST(LinkedInParserStrategy, ClassifyJsonDetailsPage)
{
    QFile file(":/json_1.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QString html = file.readAll();

    const LinkedInParserStrategy strategy;
    const PageType pageType = strategy.classifyPage(html);

    EXPECT_EQ(pageType, PageType::Details);
}

TEST(LinkedInParserStrategy, FullSummaries)
{
    QFile file(":/full_summaries.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QString html = file.readAll();

    const LinkedInParserStrategy strategy;
    const QList<Vacancy> summaries = strategy.parseSummaries(html);

    EXPECT_EQ(summaries.size(), 10);
}

TEST(LinkedInParserStrategy, PartialSummaries)
{
    QFile file(":/partial_summaries.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QString html = file.readAll();

    const LinkedInParserStrategy strategy;
    const QList<Vacancy> summaries = strategy.parseSummaries(html);
    EXPECT_EQ(summaries.size(), 6);
}

TEST(LinkedInParserStrategy, EmptySummaries)
{
    QFile file(":/empty_summaries.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QString html = file.readAll();

    const LinkedInParserStrategy strategy;
    const QList<Vacancy> summaries = strategy.parseSummaries(html);
    EXPECT_EQ(summaries.size(), 0);
}

TEST(LinkedInParserStrategy, ParseHtmlDetails)
{
    QFile file(":/html_1.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QUrl url("Html 1 url");

    const LinkedInParserStrategy strategy;
    const Vacancy vacancy = strategy.parseDetails(file.readAll(), url);

    EXPECT_EQ(vacancy.title, "Senior C++ Software Engineer (Gameplay)");
    EXPECT_EQ(vacancy.url, url);
    EXPECT_EQ(vacancy.companyName, "Playrix");
    EXPECT_EQ(vacancy.companyUrl, QUrl("https://ie.linkedin.com/company/playrix-entertainment"));
    EXPECT_EQ(vacancy.location, "Serbia");
    EXPECT_EQ(vacancy.employmentType, "Full-time");

    EXPECT_TRUE(vacancy.skills.isEmpty());

    EXPECT_EQ(vacancy.description, "<p><strong>Playrix </strong>— одна из самых успешных компаний по разработке мобильных игр, входит в топ-3 компаний-разработчиков мобильных игр в мире. Наши игры установили 2,3 миллиарда раз (треть населения Земли), каждый день в них играет более 30 миллионов людей!</p><p><br></p><p>Чтобы повышать качество продуктов и выводить их на новый уровень, мы строим <strong>AI-first-культуру </strong>внутри инженерных команд: разрабатываем агентные воркфлоу, внедряем новые инструменты, измеряем <strong>прирост скорости разработки</strong> — и в особенности следим за <strong>качеством результата. </strong></p><p>Поэтому ищем <strong>сильнейших инженеров</strong>, кто готов поддержать и развивать эти практики вместе с нами.</p><p><br></p><p>При этом опыт работы на С++ и/или в геймдеве совсем необязателен: у нас отличная система адаптации для комфортного перехода новичков с Unity и из других индустрий.</p><p><br></p><p><strong>В каждом проекте у нас есть основные направления: </strong></p><ul><li>Фичевое: разработка событий и мини-игр с нуля (от различных соревнований до мини-игр типа Merge);</li><li>Техническое: оптимизация, поддержка технической стабильности, SDK (сокращение скорости загрузки, потребление памяти и др.);</li><li>Match-3 и мета: работа с графикой и основной механикой игры (добавление нового функционала, рефакторинг и оптимизация).</li></ul><p><br></p><p><strong>Задачи, которые предстоит решать: </strong></p><ul><li>Программирование игровых механик и логики;</li><li>Проектирование и ревью архитектуры фич;</li><li>Программирование UI и визуальных эффектов;</li><li>Профилирование и оптимизация игры по памяти и FPS;</li><li>Менторство начинающих специалистов;</li><li>Проведение code review.</li></ul><p><br></p><p><strong>Наш стек:</strong></p><ul><li>Языки: С++, для единичных задач Lua и Python;</li><li>Графика: OpenGL ES и Metal;</li><li>Собственный движок, похожий на Unity или Unreal, который мы постоянно развиваем (<u>почитать о нашем движке</u>);</li><li>Продвинутый CI/CD, автоматическое тестирование, процессы code review и многое другое;</li><li>AI: Claude, Cursor, Codex с настроенными правилами репозиториев, растущим каталогом специализированных скиллов и агентских режимов под конкретные задачи. MCP-интеграции с внутренней инфраструктурой (GitHub, Asana, Amplitude и тд). Агентские воркфлоу покрывают весь цикл: от генерации игровых элементов и автотестов до автоматического создания и ревью PR.</li></ul><p><br></p><p><strong>Мы ожидаем:</strong></p><ul><li>Опыт разработки коммерческих проектов на С++ и/или Unity не менее 4 лет;</li><li>Опыт разработки проектов, выпущенных в production;</li><li>Понимание AI-инфраструктуры, современных инструментов и умение встраивать их в процесс разработки;</li><li>Опыт проектирования архитектуры.</li></ul><p><br></p><p><strong>Будет плюсом:</strong></p><ul><li>Уверенное владение современными стандартами С++ 14/17, а также STL;</li><li>Опыт игровой разработки;</li><li>Опыт работы с популярными игровыми движками (Unity, Unreal, Godot, Cocos2D, Defold и т.д.);</li><li>Опыт разработки под мобильные платформы (iOS, Android);</li><li>Опыт работы с компьютерной графикой.</li></ul><p><br></p><p><strong>Как мы работаем с AI:</strong></p><ul><li><strong>AI — стандарт инженерной работы в команде, а не опция. </strong>Используем ежедневно в разработке: от декомпозиции задач, навигации по кодовой базе до генерации и рефакторинга кода.</li><li><strong>Ответственность за результат всегда остаётся за инженером.</strong> Любой сгенерированный код проходит те же проверки, что и написанный вручную: ревью, тесты, quality gates и валидацию в контексте продукта.</li><li><strong>AI усиливает инженера, а не заменяет его.</strong> Мы ожидаем, что с помощью AI инженер сможет быстрее двигаться по задачам, брать на себя больший объём работы и эффективнее решать сложные проблемы.</li><li><strong>Делимся рабочими практиками внутри команды.</strong> Хорошие сценарии использования AI, удачные подходы, правила работы с репозиторием и агентные воркфлоу становятся частью общей инженерной среды.</li><li><strong>Постоянно улучшаем подход.</strong> Инструменты быстро меняются, поэтому мы ожидаем интереса к новым возможностям AI и готовности предлагать улучшения, если они реально повышают скорость и качество разработки.</li></ul><p><br></p><p><strong>У нас классно, потому что у нас есть:</strong></p><ul><li>Задачи, требующие умения принимать эффективные и сложные архитектурные решения, а также применять на практике интересные подходы программирования.</li><li>Возможность карьерно расти и/или переходить между направлениями и не только в рамках разработки (Project Management, маркетинг и т.д.).</li><li>Упор на развитие продуктового видения у каждого специалиста в компании. Наши разработчики представляют, как та или иная фича будет работать у пользователя, и сами играют в игры, которые разрабатывают.</li><li>Возможность менять процессы и подходы на более эффективные без долгих согласований и бюрократии.</li></ul><p><br></p><p><strong>Мы предлагаем:</strong></p><p><strong>Гибкость в работе:</strong> Для большинства позиций у нас гибкий график и удалённый формат работы. Мы ориентируемся только на результат.</p><p><strong>Забота о здоровье и well-being:</strong></p><ul><li>Предоставляем ДМС для сотрудников, их партнеров и детей, компенсируем онлайн-сессии с психологом, включая индивидуальные, семейные и детские консультации.</li><li>Сохраняем 100% зарплату во время отпуска или больничного без лишних справок. А для особых случаев предоставляем дополнительные выходные.</li><li>Поддерживаем здоровый образ жизни и компенсируем покупку спортивных абонементов и подписку на фитнес-приложения, а также массаж, посещение SPA и восстановительные процедуры.</li></ul><p><strong>Развитие и обучение:</strong></p><ul><li>Оплачиваем участие в профильных конференциях и курсах, проводим внутренние конференции.</li><li>Компенсируем изучение иностранного языка.</li><li>Для сотрудников, совмещающих работу с обучением, предоставляем дополнительные оплачиваемые дни для экзаменов, защиты диплома и других обязательных учебных мероприятий.</li></ul><p><strong>Ивенты и мерч:</strong></p><ul><li>Регулярно проводим развлекательные и рабочие мероприятия в офлайн режиме: встречи команд, крупные корпоративные вечеринки и более лайтовые пати в офисах.</li><li>Организовываем развлекательные конкурсы, квизы, хакатоны, спортивные челленджи в онлайн формате.</li><li>Собственный магазин мерча с внутренней валютой, где можно купить уникальные подарки - например, новую приставку или ракетку для падела.</li></ul><p><strong>Забота о питомцах.</strong> Компенсируем часть расходов на ветеринарные услуги и другие траты на домашних животных.</p><p><br></p><p><br></p><p><em>Отправляя отклик на нашу вакансию, вы соглашаетесь, что ваши персональные данные, собранные во время собеседований в Playrix, будут обрабатываться в соответствии с условиями (https://playrix.com/candidate-privacy). Вы можете отправить запрос в отношении ваших персональных данных по адресу: job@playrix.com.</em></p>");

    EXPECT_FALSE(vacancy.date.isValid());
    EXPECT_FALSE(vacancy.industry.has_value());

    EXPECT_EQ(vacancy.level.value(), "Mid-Senior level");

    EXPECT_FALSE(vacancy.experience.has_value());
    EXPECT_FALSE(vacancy.education.has_value());
    EXPECT_FALSE(vacancy.expirationDate.has_value());
}

TEST(LinkedInParserStrategy, ParseJsonDetails)
{
    QFile file(":/json_1.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QUrl url("json 1 url");

    const LinkedInParserStrategy strategy;
    const Vacancy vacancy = strategy.parseDetails(file.readAll(), url);

    EXPECT_EQ(vacancy.title, "Senior Software Engineer (C++) for JVM runtime");
    EXPECT_EQ(vacancy.url, url);
    EXPECT_EQ(vacancy.companyName, "Azul");
    EXPECT_EQ(vacancy.companyUrl, QUrl("https://www.linkedin.com/company/azul-systems"));
    EXPECT_EQ(vacancy.location, "Belgrade");
    EXPECT_EQ(vacancy.employmentType, "FULL_TIME");

    EXPECT_TRUE(vacancy.skills.isEmpty());

    EXPECT_EQ(vacancy.description, "<p>We are seeking a highly skilled **Senior Software Engineer** to join our team developing Azul Prime, an award-winning enhanced build of OpenJDK designed for superior application performance, responsiveness, and efficiency. In this role, you will drive the development and optimization of the virtual machine (VM), contributing to garbage collection, HotSpot runtime, compiler runtime, remote compilation, and many other advanced challenges.</p><p><p><br></p><strong>What You'll Do</strong><ul><br><li>Design, develop, maintain, and optimize high‑performance, low‑level C++ code for system‑critical JVM runtime components and distributed client‑server communication using gRPC, HTTP/2, or HTTP, ensuring minimal overhead, robustness, and reliability at scale.<br><br></li><br><li>Lead the delivery of complex technical projects — from design through production — taking ownership of outcomes and navigating ambiguity under real time and resource constraints.<br><br></li><br><li>Contribute to the technical direction of the team: participate in design discussions, challenge and pressure-test technical approaches, and ensure robust solutions through constructive peer review.<br><br></li><br><li>Collaborate across teams — represent the team's technical interests in cross-team initiatives and contribute as a peer in design and direction discussions with partner engineering groups.<br><br></li><br><li>Mentor junior engineers, sharing expertise through reviews, pairing, and practical guidance that raises the quality bar across the team.<br><br></li><br><li>Explore and adopt AI-first workflows — proactively identifying where AI tools can enhance productivity, code quality, and development processes, and helping teammates adopt effective practices.<br><br></li><br><li>Perform detailed debugging, profiling, and root-cause analysis of complex software issues.<br><br></li><br><li>Develop comprehensive tests to ensure high quality and demonstrate performance improvements.</li><br></ul><p><br></p><strong>Wha you'll Need</strong><ul><br><li>Bachelor's degree in Computer Science or a related field, and a minimum of 5 years of experience writing low‑level, high‑performance C++ code.<br><br></li><br><li>Solid knowledge of VM and compiler runtime design and implementation; familiarity with JVM internals is highly desirable.<br><br></li><br><li>Demonstrated ability to independently lead and deliver complex technical projects on time, managing competing priorities and technical uncertainty.<br><br></li><br><li>Experience mentoring engineers and contributing to a team's technical culture and practices.<br><br></li><br><li>Strong proficiency with low‑level programming concepts — memory management, pointers, concurrency, and multithreading.<br><br></li><br><li>Experience using debuggers, profilers, and performance analysis tools to investigate and resolve complex problems.<br><br></li><br><li>Understanding of hardware architectures (e.g. x86, ARM), operating system internals (e.g. Linux, macOS), and languages such as Java or Rust is an advantage.</li><br></ul><p><br></p><strong>We Offer</strong><br><ul><br><li><br><p>Referral Program — help us grow with great people.</p><br></li><br><li><br><p>Premium IT equipment — MacBook Pro or hardware of your choice.</p><br></li><br><li><br><p>5 weeks of holidays + 5 sick days.</p><br></li><br><li><br><p>Flexible working hours; full remote possible.</p><br></li><br><li><br><p>Prague/Limassol/Belgrade city center offices. Hybrid or full-remote also possible.</p><br></li><br><li><br><p>Work alongside top experts worldwide contributing to the Java ecosystem.</p><br></li><br></ul><br></p><p><br></p><br><p>Employment at Azul is contingent upon the completion of reference and background checks.</p><br><p>We may use artificial intelligence (AI) tools to support parts of the hiring process, such as reviewing applications, analyzing resumes, or assessing responses and identifying potential inconsistencies or verification signals in application materials based on available information. These tools assist our recruitment team but do not replace human judgment. Final hiring decisions are ultimately made by humans. If you would like more information about how your data is processed, please contact us.</p>");
    EXPECT_EQ(vacancy.date, QDate::fromString("2026-09-27", Qt::ISODate));
    EXPECT_EQ(vacancy.industry.value(), "Software Development,Technology, Information and Media,IT Services and IT Consulting");

    EXPECT_FALSE(vacancy.level.has_value());

    EXPECT_EQ(vacancy.experience.value(), 60);
    EXPECT_EQ(vacancy.education.value(), "bachelor degree");
    EXPECT_EQ(vacancy.expirationDate.value(), QDate::fromString("2026-12-08", Qt::ISODate));
}

TEST(LinkedInParserStrategy, ParseDetailsFallsBackToHtmlWhenJsonIsInvalid)
{
    const QString html = R"HTML(
        <h1 class="top-card-layout__title">Software Engineer</h1>
        <a class="topcard__org-name-link" href="https://example.com/company/acme?trk=job">Acme</a>
        <script type="application/ld+json">{ invalid json }</script>
    )HTML";

    const QUrl url("https://example.com/jobs/1");
    const LinkedInParserStrategy strategy;

    const Vacancy vacancy = strategy.parseDetails(html, url);

    EXPECT_EQ(vacancy.title, "Software Engineer");
    EXPECT_EQ(vacancy.url, url);
    EXPECT_EQ(vacancy.companyName, "Acme");
    EXPECT_EQ(vacancy.companyUrl, QUrl("https://example.com/company/acme"));
}
