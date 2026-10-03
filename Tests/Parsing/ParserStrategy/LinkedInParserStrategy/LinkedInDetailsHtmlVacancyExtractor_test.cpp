#include <gtest/gtest.h>

#include <QFile>

#include <LinkedInParserStrategy/LinkedInDetailsHtmlVacancyExtractor.h>

TEST(LinkedInDetailsHtmlVacancyExtractor, Html1)
{
    QFile file(":/html_1.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QUrl url("Html 1 url");

    const LinkedInDetailsHtmlVacancyExtractor extractor(file.readAll(), url);

    const Vacancy vacancy = extractor.vacancy();

    EXPECT_EQ(vacancy.title, "Senior C++ Software Engineer (Gameplay)");
    EXPECT_EQ(vacancy.link, url);
    EXPECT_EQ(vacancy.companyName, "Playrix");
    EXPECT_EQ(vacancy.companyLink, QUrl("https://ie.linkedin.com/company/playrix-entertainment"));
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

TEST(LinkedInDetailsHtmlVacancyExtractor, Html2)
{
    QFile file(":/html_2.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QUrl url("html 2 url");

    const LinkedInDetailsHtmlVacancyExtractor extractor(file.readAll(), url);

    const Vacancy vacancy = extractor.vacancy();

    EXPECT_EQ(vacancy.title, "Senior Open Source Engineer");
    EXPECT_EQ(vacancy.link, url);
    EXPECT_EQ(vacancy.companyName, "Sysdig");
    EXPECT_EQ(vacancy.companyLink, QUrl("https://www.linkedin.com/company/sysdig"));
    EXPECT_EQ(vacancy.location, "Serbia");
    EXPECT_EQ(vacancy.employmentType, "Full-time");

    EXPECT_TRUE(vacancy.skills.isEmpty());

    EXPECT_EQ(vacancy.description, "In the cloud, every second counts. On the leading edge of security, Sysdig stops attacks in real-time by instantly detecting changes in cloud security risk with runtime insights and open source Falco. Trusted by a large enterprise customer base, we are a well-funded startup, passionate open source enthusiasts at heart, and problem-solvers who are building and delivering powerful solutions to secure cloud-native applications.<br><br>We have an organizational focus on delivering value to customers. We appreciate diverse opinions and open dialogue to spur ideas and we believe in working together to achieve our goals. We're an international company that understands how to cultivate an inclusive environment across all teams.<br><br>And we're a great place to work too - we've been named a \"Best Places to Work\" by Inc, the San Francisco Business Time, and Built In, with recognitions ranging from \"Best Benefits\" to a \"Best Company for Happiness\".<br><br>We are looking for driven team members who want to join us on our mission to lead cloud security globally. Does this sound like the right place for you?<br><br><strong>What You Will Do<br><br></strong><ul><li>You will develop exciting pieces of new technology within Falco, OSS Sysdig, their libraries, and our Open Source projects</li><li>You will become a contributor and possibly a maintainer of these open source projects</li><li>You will help users and adopters get up to speed with these tools and help fix reported bugs</li><li>You will be responsible for making releases and working on modern automation</li><li>You will engage with the community via Slack and GitHub, participate in conferences like Kubecon, deliver technical talks, and write blogs on open source technologies, such as the Linux kernel, Falco, and Sysdig<br><br></li></ul><strong>What You Will Bring With You<br><br></strong><ul><li>Solid programming skills in the following programming languages: C/C++, Golang, Rust</li><li>Contribution to open source projects</li><li>Linux, eBPF, Containers, and Kubernetes experience<br><br></li></ul><strong>What We Look For<br><br></strong><ul><li>5 years of experience as Software Engineer in cloud native or security domains</li><li>Excellent analytical and problem-solving skills</li><li>Strong communication and collaboration skills<br><br></li></ul><strong>When You Join Sysdig, You Can Expect<br><br></strong><ul><li>Extra days off to prioritize your well-being</li><li>Mental health support for you and your family through the Modern Health app</li><li>Great compensation package<br><br></li></ul>We would love for you to join us! Please reach out even if your experience doesn't perfectly match the job description. We can always explore other options after starting the conversation. Your background and passion will set you apart, especially if your career path is different.<br><br>Some of our Hiring Managers are globally distributed, an English version of your CV will be appreciated.<br><br>Sysdig values a diverse workplace and encourages women, people of color, LGBTQIA+ individuals, people with disabilities, members of ethnic minorities, foreign-born residents, and veterans to apply. Sysdig is an equal-opportunity employer. Sysdig does not discriminate on the basis of race, color, religion, sex, national origin, age, disability, genetic information, sexual orientation, gender identity, or any other legally protected status.<br><br>Sysdig may use responsible AI tools in connection with its recruitment and hiring efforts. These tools may be used to help Sysdig identify and shortlist talent for current and future job postings. You can opt-out of this practice at any time by [email protected] . To learn more about how this works and your rights, please see the Sysdig Global Applicant and Employee Privacy Notice: https://cdn.prod.website-files.com/681a1c8e5b6ebfc0f8529533/683d7a57fc05b32bae08c0f9_sysdig-global-employee-privacy-notice.pdf<br><br>");

    EXPECT_FALSE(vacancy.date.isValid());
    EXPECT_FALSE(vacancy.industry.has_value());

    EXPECT_EQ(vacancy.level.value(), "Mid-Senior level");

    EXPECT_FALSE(vacancy.experience.has_value());
    EXPECT_FALSE(vacancy.education.has_value());
    EXPECT_FALSE(vacancy.expirationDate.has_value());
}

TEST(LinkedInDetailsHtmlVacancyExtractor, Json1)
{
    QFile file(":/json_1.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QUrl url("json 1 url");

    const LinkedInDetailsHtmlVacancyExtractor extractor(file.readAll(), url);

    const Vacancy vacancy = extractor.vacancy();

    EXPECT_EQ(vacancy.title, "Senior Software Engineer (C++) for JVM runtime");
    EXPECT_EQ(vacancy.link, url);
    EXPECT_EQ(vacancy.companyName, "Azul");
    EXPECT_EQ(vacancy.companyLink, QUrl("https://www.linkedin.com/company/azul-systems"));
    EXPECT_EQ(vacancy.location, "Belgrade, Serbia");
    EXPECT_EQ(vacancy.employmentType, "Full-time");

    EXPECT_TRUE(vacancy.skills.isEmpty());

    EXPECT_EQ(vacancy.description, "<p>We are seeking a highly skilled **Senior Software Engineer** to join our team developing Azul Prime, an award-winning enhanced build of OpenJDK designed for superior application performance, responsiveness, and efficiency. In this role, you will drive the development and optimization of the virtual machine (VM), contributing to garbage collection, HotSpot runtime, compiler runtime, remote compilation, and many other advanced challenges.</p><p><p><br></p><strong>What You'll Do</strong><ul><br><li>Design, develop, maintain, and optimize high‑performance, low‑level C++ code for system‑critical JVM runtime components and distributed client‑server communication using gRPC, HTTP/2, or HTTP, ensuring minimal overhead, robustness, and reliability at scale.<br><br></li><br><li>Lead the delivery of complex technical projects — from design through production — taking ownership of outcomes and navigating ambiguity under real time and resource constraints.<br><br></li><br><li>Contribute to the technical direction of the team: participate in design discussions, challenge and pressure-test technical approaches, and ensure robust solutions through constructive peer review.<br><br></li><br><li>Collaborate across teams — represent the team's technical interests in cross-team initiatives and contribute as a peer in design and direction discussions with partner engineering groups.<br><br></li><br><li>Mentor junior engineers, sharing expertise through reviews, pairing, and practical guidance that raises the quality bar across the team.<br><br></li><br><li>Explore and adopt AI-first workflows — proactively identifying where AI tools can enhance productivity, code quality, and development processes, and helping teammates adopt effective practices.<br><br></li><br><li>Perform detailed debugging, profiling, and root-cause analysis of complex software issues.<br><br></li><br><li>Develop comprehensive tests to ensure high quality and demonstrate performance improvements.</li><br></ul><p><br></p><strong>Wha you'll Need</strong><ul><br><li>Bachelor's degree in Computer Science or a related field, and a minimum of 5 years of experience writing low‑level, high‑performance C++ code.<br><br></li><br><li>Solid knowledge of VM and compiler runtime design and implementation; familiarity with JVM internals is highly desirable.<br><br></li><br><li>Demonstrated ability to independently lead and deliver complex technical projects on time, managing competing priorities and technical uncertainty.<br><br></li><br><li>Experience mentoring engineers and contributing to a team's technical culture and practices.<br><br></li><br><li>Strong proficiency with low‑level programming concepts — memory management, pointers, concurrency, and multithreading.<br><br></li><br><li>Experience using debuggers, profilers, and performance analysis tools to investigate and resolve complex problems.<br><br></li><br><li>Understanding of hardware architectures (e.g. x86, ARM), operating system internals (e.g. Linux, macOS), and languages such as Java or Rust is an advantage.</li><br></ul><p><br></p><strong>We Offer</strong><br><ul><br><li><br><p>Referral Program — help us grow with great people.</p><br></li><br><li><br><p>Premium IT equipment — MacBook Pro or hardware of your choice.</p><br></li><br><li><br><p>5 weeks of holidays + 5 sick days.</p><br></li><br><li><br><p>Flexible working hours; full remote possible.</p><br></li><br><li><br><p>Prague/Limassol/Belgrade city center offices. Hybrid or full-remote also possible.</p><br></li><br><li><br><p>Work alongside top experts worldwide contributing to the Java ecosystem.</p><br></li><br></ul><br></p><p><br></p><br><p>Employment at Azul is contingent upon the completion of reference and background checks.</p><br><p>We may use artificial intelligence (AI) tools to support parts of the hiring process, such as reviewing applications, analyzing resumes, or assessing responses and identifying potential inconsistencies or verification signals in application materials based on available information. These tools assist our recruitment team but do not replace human judgment. Final hiring decisions are ultimately made by humans. If you would like more information about how your data is processed, please contact us.</p>");

    EXPECT_FALSE(vacancy.date.isValid());
    EXPECT_FALSE(vacancy.industry.has_value());

    EXPECT_EQ(vacancy.level.value(), "Mid-Senior level");

    EXPECT_FALSE(vacancy.experience.has_value());
    EXPECT_FALSE(vacancy.education.has_value());
    EXPECT_FALSE(vacancy.expirationDate.has_value());
}

TEST(LinkedInDetailsHtmlVacancyExtractor, Json2)
{
    QFile file(":/json_2.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QUrl url("json 2 url");

    const LinkedInDetailsHtmlVacancyExtractor extractor(file.readAll(), url);

    const Vacancy vacancy = extractor.vacancy();

    EXPECT_EQ(vacancy.title, "C++ Software Engineer – SDK System & Performance");
    EXPECT_EQ(vacancy.link, url);
    EXPECT_EQ(vacancy.companyName, "Cisco");
    EXPECT_EQ(vacancy.companyLink, QUrl("https://www.linkedin.com/company/cisco"));
    EXPECT_EQ(vacancy.location, "Belgrade, Serbia");
    EXPECT_EQ(vacancy.employmentType, "Full-time");

    EXPECT_TRUE(vacancy.skills.isEmpty());

    EXPECT_EQ(vacancy.description, "<strong>Meet the Team<br><br></strong>We, at CISCO Silicon One™, are building SDK, firmware, simulator, and compiler for the top-of-the-line Networking ASIC.<br><br>The SDK System & Performance team manages all of SDK features that are device agnostic. This involves intricate C++ projects, that requires proficiency and intuition of C++, compiler, linker, loader, and general Linux program runtime environment and subsystems. You will improve SDK code quality, manage SDK threads, and various common SDK tools and utilities.<br><br>We manage SDK memory footprint and debuggability, which involves deep understanding of the Linux memory subsystem. You will profile, analyse, propose and implement solutions to improve SDK memory usage, and make it more robust.<br><br>One of the major cross device functionalities and engineering challenges is supporting SDK critical updates on live systems. You will work closely with major CISCO customers to enable and validate these functionalities.<br><br>We are looking for people who are in love with technology and engineering to help us push the limits of what is possible through innovation, determination, and teamwork.<br><br><strong>Your Impact<br><br></strong><ul><li>Design and deliver core, cross-device SDK infrastructure in modern C++ (C++14/C++17), with a strong focus on performance, scalability, and long-term maintainability</li><li>Drive system-level engineering across the build and runtime stack, including deep work with the compiler, linker, loader, and binary/layout considerations</li><li>Own and improve the SDK’s memory footprint and debuggability, building mechanisms that increase observability and reliability in production environments</li><li>Diagnose complex issues across compile-time and runtime (performance regressions, memory behaviour, and hard-to-reproduce failures) and implement durable fixes</li><li>Develop and maintain low-level tooling and debug layers (e.g., allocation tracking/interposition) to support troubleshooting and serviceability</li><li>Collaborate with partner teams to shape SDK architecture, design patterns, and engineering practices that enable robust platform operation and critical update workflows<br><br></li></ul>The result of your work will be integrated into the products of world-leading companies and will have a worldwide impact.<br><br><strong>Minimum Qualifications<br><br></strong><ul><li>3+ years of programming experience in C++ and system programming</li><li>Solid knowledge of Linux</li><li>Academic education in computer technology (finished BSc/MSc studies)</li><li>Fluency in spoken and written English<br><br></li></ul><strong>Preferred Qualifications<br><br></strong><ul><li>Solid understanding of hardware architecture and its interaction with software.</li><li>Passion for SW architecture.</li><li>Good problem-solving, debugging, and analytical skills.</li><li>Experience with other programming languages and tools such as Python.<br><br></li></ul>Why Cisco?<br><br>At Cisco, we’re revolutionizing how data and infrastructure connect and protect organizations in the AI era – and beyond. We’ve been innovating fearlessly for 40 years to create solutions that power how humans and technology work together across the physical and digital worlds. These solutions provide customers with unparalleled security, visibility, and insights across the entire digital footprint.<br><br>Fueled by the depth and breadth of our technology, we experiment and create meaningful solutions. Add to that our worldwide network of doers and experts, and you’ll see that the opportunities to grow and build are limitless. We work as a team, collaborating with empathy to make really big things happen on a global scale. Because our solutions are everywhere, our impact is everywhere.<br><br>We are Cisco, and our power starts with you.");

    EXPECT_FALSE(vacancy.date.isValid());
    EXPECT_FALSE(vacancy.industry.has_value());

    EXPECT_EQ(vacancy.level.value(), "Mid-Senior level");

    EXPECT_FALSE(vacancy.experience.has_value());
    EXPECT_FALSE(vacancy.education.has_value());
    EXPECT_FALSE(vacancy.expirationDate.has_value());
}